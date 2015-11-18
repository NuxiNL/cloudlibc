// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <argdata.h>
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

struct iterate_data {
  bool first;
  FILE *fp;
  unsigned int depth;
};

static void print_yaml(const argdata_t *, FILE *, unsigned int);

static void print_space(unsigned int depth, FILE *fp) {
  fprintf(fp, "\n%*s", depth, "");
}

// Prints the elements stored in a map.
static bool iterate_map(const argdata_t *key, const argdata_t *value,
                        void *thunk) {
  struct iterate_data *id = thunk;
  if (id->first) {
    fputs("!!map {", id->fp);
    id->first = false;
  }
  print_space(id->depth, id->fp);
  fputs("? ", id->fp);
  print_yaml(key, id->fp, id->depth);
  print_space(id->depth, id->fp);
  fputs(": ", id->fp);
  print_yaml(value, id->fp, id->depth);
  fputc(',', id->fp);
  return true;
}

// Prints the elements stored in a sequence.
static bool iterate_seq(const argdata_t *ad, void *thunk) {
  struct iterate_data *id = thunk;
  if (id->first) {
    fputs("!!seq [", id->fp);
    id->first = false;
  }
  print_space(id->depth, id->fp);
  print_yaml(ad, id->fp, id->depth);
  fputc(',', id->fp);
  return true;
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
    if (argdata_get_fd(ad, &value) == 0) {
      fprintf(fp, "!fd \"%d\"", value);
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
    struct iterate_data id = {.first = true, .fp = fp, .depth = depth + 2};
    if (argdata_iterate_map(ad, iterate_map, &id) == 0 || !id.first) {
      if (id.first) {
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
    struct iterate_data id = {.first = true, .fp = fp, .depth = depth + 2};
    if (argdata_iterate_seq(ad, iterate_seq, &id) == 0 || !id.first) {
      if (id.first) {
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
      locale_t locale = LC_C_UNICODE_LOCALE;
      mbstate_t mbs;
      memset(&mbs, '\0', sizeof(mbs));
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
