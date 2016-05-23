// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdint.h>
#include <stdlib.h>

long a64l(const char *s) {
  int32_t value = 0;
  for (size_t i = 0; i < 6; ++i) {
    char c = s[i];
    int32_t digit;
    if (c == '.') {
      digit = 0;
    } else if (c == '/') {
      digit = 1;
    } else if (c >= '0' && c <= '9') {
      digit = c - '0' + 2;
    } else if (c >= 'A' && c <= 'Z') {
      digit = c - 'A' + 12;
    } else if (c >= 'a' && c <= 'z') {
      digit = c - 'a' + 38;
    } else {
      break;
    }
    value |= digit << i * 6;
  }
  return value;
}
