// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <stdlib.h>

int l64a_r(long value, char *buffer, int buflen) {
  uint32_t v = value;
  while (buflen-- > 0) {
    if (v == 0) {
      *buffer = '\0';
      return 0;
    }
    *buffer++ =
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
            [v & 0x3f];
    v >>= 6;
  }
  return -1;
}
