// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <argdata.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

static int print_yaml(const argdata_t *ad, FILE *fp, unsigned int depth) {
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
      return 0;
    }
  }

  // Booleans.
  {
    bool value;
    if (argdata_get_bool(ad, &value) == 0) {
      fputs(value ? "!!bool \"true\"" : "!!bool \"false\"", fp);
      return 0;
    }
  }

  fputs("!!null \"null\"", fp);
  return 0;
}

int argdata_print_yaml(const argdata_t *ad, FILE *fp) {
  fputs("---\n", fp);
  int error = print_yaml(ad, fp, 0);
  if (error != 0)
    return error;
  fputc('\n', fp);
  return 0;
}
