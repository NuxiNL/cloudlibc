// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdint.h>
#include <stdlib.h>

int l64a_r(long value, char *buffer, int buflen) {
  uint32_t v = value;
  for (;;) {
    if (buflen <= 0)
      return -1;
    if (v == 0) {
      *buffer = '\0';
      return 0;
    }
    *buffer++ =
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
            [v & 0x3f];
    --buflen;
    v >>= 6;
  }
}
