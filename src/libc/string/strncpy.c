// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>
#include <cloudlibc_interceptors.h>

char *__cloudlibc_strncpy(char *restrict s1, const char *restrict s2, size_t n) {
  char *begin = s1;
  while (n > 0 && *s2 != '\0') {
    *s1++ = *s2++;
    --n;
  }
  while (n-- > 0)
    *s1++ = '\0';
  return begin;
}

__weak_reference(__cloudlibc_strncpy, strncpy);
