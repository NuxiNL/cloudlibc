// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <argdata.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

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
      fprintf(fp, "!!fd \"%d\"", value);
      return;
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
    if (argdata_iterate_map(ad, &id, iterate_map) == 0 || !id.first) {
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
    if (argdata_iterate_seq(ad, &id, iterate_seq) == 0 || !id.first) {
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
      // TODO(edje): Add proper escaping.
      fprintf(fp, "!!str \"%s\"", buf);
      return;
    }
  }

  // Unrepresentable object.
  fputs("!!null \"null\"", fp);
}

void argdata_print_yaml(const argdata_t *ad, FILE *fp) {
  fputs("---\n", fp);
  print_yaml(ad, fp, 0);
  fputc('\n', fp);
}
