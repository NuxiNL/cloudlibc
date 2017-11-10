// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <string.h>

char *strcpy(char *restrict s1, const char *restrict s2) {
  char *s = s1;
  for (;;) {
    *s++ = *s2;
    if (*s2++ == '\0')
      return s1;
  }
}
