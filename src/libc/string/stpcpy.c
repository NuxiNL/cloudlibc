// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <string.h>

char *stpcpy(char *restrict s1, const char *restrict s2) {
  for (;;) {
    *s1 = *s2;
    if (*s2++ == L'\0')
      return s1;
    ++s1;
  }
}
