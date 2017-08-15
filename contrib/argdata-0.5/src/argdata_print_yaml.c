// Copyright (c) 2015-2017 Nuxi (https://nuxi.nl/) and contributors.
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <float.h>
#include <inttypes.h>
#include <locale.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>

#include "argdata.h"

#ifndef DBL_DECIMAL_DIG
#define DBL_DECIMAL_DIG DECIMAL_DIG
#endif

static void print_space(unsigned int depth, FILE *fp) {
  fprintf(fp, "\n%*s", depth, "");
}

// Recursively prints a node as YAML.
static void print_yaml(const argdata_t *ad, FILE *fp, unsigned int depth) {
  // Binary data. Print using Base64 encoding.
  {
    const void *buf;
    size_t len;
    if (argdata_get_binary(ad, &buf, &len) == 0) {
      fputs("!!binary \"", fp);
      const uint8_t *b = buf;
      static const char base64[64] =
          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

      // Print data in groups of three bytes.
      while (len >= 3) {
        char b64[4] = {base64[b[0] >> 2], base64[(b[0] & 0x3) << 4 | b[1] >> 4],
                       base64[(b[1] & 0xf) << 2 | b[2] >> 6],
                       base64[b[2] & 0x3f]};
        fwrite(b64, 1, sizeof(b64), fp);
        b += 3;
        len -= 3;
      }

      // Print trailing group of one or two bytes.
      if (len == 1) {
        char b64[4] = {base64[b[0] >> 2], base64[(b[0] & 0x3) << 4], '=', '='};
        fwrite(b64, 1, sizeof(b64), fp);
      } else if (len == 2) {
        char b64[4] = {base64[b[0] >> 2], base64[(b[0] & 0x3) << 4 | b[1] >> 4],
                       base64[(b[1] & 0xf) << 2], '='};
        fwrite(b64, 1, sizeof(b64), fp);
      }
      fputc('"', fp);
      return;
    }
  }

  // Booleans.
  {
    bool value;
    if (argdata_get_bool(ad, &value) == 0) {
      fputs(value ? "!!bool \"true\"" : "!!bool \"false\"", fp);
      return;
    }
  }

  // Extension: file descriptors.
  {
    int value;
    switch (argdata_get_fd(ad, &value)) {
      case 0:
        fprintf(fp, "!fd \"%d\"", value);
        return;
      case EBADF:
        fprintf(fp, "!fd \"invalid\"");
        return;
    }
  }

  // Floating point numbers.
  {
    double value;
    if (argdata_get_float(ad, &value) == 0) {
      switch (fpclassify(value)) {
        case FP_INFINITE:
          // Positive or negative infinity.
          fprintf(fp,
                  signbit(value) ? "!!float \"-.inf\"" : "!!float \".inf\"");
          return;
        case FP_NAN:
          // Not a number.
          fprintf(fp, "!!float \".nan\"");
          return;
        case FP_ZERO:
          // Zero. YAML doesn't distinguish between positive and negative zero.
          fprintf(fp, "!!float \"0\"");
          return;
        default: {
          // Normal or subnormal value.
          char num[DBL_DECIMAL_DIG + 32];
          snprintf(num, sizeof(num), "%.*e", DBL_DECIMAL_DIG, value);
          char *exp = strchr(num, 'e');
          // Strip off trailing zero digits from the significand.
          for (char *dig = exp - 1; dig >= num && (*dig == '0' || *dig == '.');
               --dig)
            *dig = '\0';
          // Preserve sign character.
          *exp++ = '\0';
          char sign = *exp++;
          // Strip off leading zeroes from the exponent.
          while (*exp == '0')
            ++exp;
          // Omit the exponent if it is zero.
          if (*exp == '\0') {
            fprintf(fp, "!!float \"%s\"", num);
          } else {
            fprintf(fp, "!!float \"%se%c%s\"", num, sign, exp);
          }
          return;
        }
      }
    }
  }

  // Signed integers.
  {
    intmax_t value;
    if (argdata_get_int(ad, &value) == 0) {
      fprintf(fp, "!!int \"%jd\"", value);
      return;
    }
  }

  // Unsigned integers.
  {
    uintmax_t value;
    if (argdata_get_int(ad, &value) == 0) {
      fprintf(fp, "!!int \"%ju\"", value);
      return;
    }
  }

  // Maps.
  {
    argdata_map_iterator_t it;
    argdata_map_iterate(ad, &it);
    if (it.index != ARGDATA_ITERATOR_INVALID) {
      const argdata_t *key;
      const argdata_t *value;
      size_t index = 0;
      while (argdata_map_get(&it, &key, &value)) {
        if (index == 0)
          fputs("!!map {", fp);
        print_space(depth + 2, fp);
        fputs("? ", fp);
        print_yaml(key, fp, depth + 2);
        print_space(depth + 2, fp);
        fputs(": ", fp);
        print_yaml(value, fp, depth + 2);
        fputc(',', fp);
        argdata_map_next(&it);
        ++index;
      }
      if (index == 0) {
        // Empty map.
        fputs("!!map {}", fp);
      } else {
        // Non-empty map.
        print_space(depth, fp);
        fputc('}', fp);
      }
      return;
    }
  }

  // Sequences.
  {
    argdata_seq_iterator_t it;
    argdata_seq_iterate(ad, &it);
    if (it.index != ARGDATA_ITERATOR_INVALID) {
      const argdata_t *value;
      size_t index = 0;
      while (argdata_seq_get(&it, &value)) {
        if (index == 0)
          fputs("!!seq [", fp);
        print_space(depth + 2, fp);
        print_yaml(value, fp, depth + 2);
        fputc(',', fp);
        argdata_seq_next(&it);
        ++index;
      }
      if (index == 0) {
        // Empty sequence.
        fputs("!!seq []", fp);
      } else {
        // Non-empty sequence.
        print_space(depth, fp);
        fputc(']', fp);
      }
      return;
    }
  }

  // Strings.
  {
    const char *buf;
    size_t len;
    if (argdata_get_str(ad, &buf, &len) == 0) {
      fputs("!!str \"", fp);
#ifdef LC_C_UNICODE_LOCALE
      locale_t locale = LC_C_UNICODE_LOCALE;
      static const mbstate_t initial_mbstate;
      mbstate_t mbs = initial_mbstate;
      while (len > 0) {
        wchar_t wc;
        ssize_t clen = mbrtowc_l(&wc, buf, len, &mbs, locale);
        if (clen <= 0) {
          wc = L'\0';
          clen = 1;
        }

        switch (wc) {
#define MAP(c, ch)                   \
  case (c): {                        \
    char cb[3] = {'\\', (ch), '\0'}; \
    fputs(cb, fp);                   \
    break;                           \
  }
          // Characters that have a shorthand escape sequence. Forward
          // slash has been omitted, as it is apparently optional.
          MAP('\0', '0');
          MAP('\a', 'a');
          MAP('\b', 'b');
          MAP('\t', 't');
          MAP('\n', 'n');
          MAP('\v', 'v');
          MAP('\f', 'f');
          MAP('\r', 'r');
          MAP(0x1b, 'e');
          MAP('"', '"');
          MAP('\\', '\\');
          MAP(0x85, 'N');
          MAP(0xa0, '_');
          MAP(0x2028, 'L');
          MAP(0x2029, 'P');
#undef MAP
          default:
            if (iswprint_l(wc, locale)) {
              // Character is printable.
              fwrite(buf, 1, clen, fp);
            } else if (wc <= UINT8_MAX) {
              fprintf(fp, "\\x%02" PRIx8, (uint8_t)wc);
            } else if (wc <= UINT16_MAX) {
              fprintf(fp, "\\u%04" PRIx16, (uint16_t)wc);
            } else {
              fprintf(fp, "\\U%08" PRIx32, (uint32_t)wc);
            }
        }
        buf += clen;
        len -= clen;
      }
#else
// TODO(ed): Make printing of strings work properly.
#endif
      fputc('"', fp);
      return;
    }
  }

  // Timestamps.
  {
    struct timespec value;
    struct tm tm;
    if (argdata_get_timestamp(ad, &value) == 0 &&
        gmtime_r(&value.tv_sec, &tm) != NULL) {
      fprintf(fp, "!!timestamp \"%04d-%02d-%02dT%02d:%02d:%02d.%09ldZ\"",
              tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour,
              tm.tm_min, tm.tm_sec, value.tv_nsec);
      return;
    }
  }

  // Unrepresentable object.
  fputs("!!null \"null\"", fp);
}

void argdata_print_yaml(const argdata_t *ad, FILE *fp) {
  fputs(
      "%TAG ! tag:nuxi.nl,2015:cloudabi/\n"
      "---\n",
      fp);
  print_yaml(ad, fp, 0);
  fputc('\n', fp);
}
