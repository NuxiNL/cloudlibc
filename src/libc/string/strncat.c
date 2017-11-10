// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <string.h>

char *strncat(char *restrict s1, const char *restrict s2, size_t n) {
  if (n > 0) {
    char *s = s1;
    while (*s != '\0')
      ++s;
    do {
      if (*s2 == '\0')
        break;
      *s++ = *s2++;
    } while (--n > 0);
    *s = '\0';
  }
  return s1;
}
