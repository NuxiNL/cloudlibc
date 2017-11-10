// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

char *stpncpy(char *restrict s1, const char *restrict s2, size_t n) {
  while (n > 0 && *s2 != '\0') {
    *s1++ = *s2++;
    --n;
  }
  char *end = s1;
  while (n-- > 0)
    *s1++ = '\0';
  return end;
}
