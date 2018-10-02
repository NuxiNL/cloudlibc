// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>
#include <cloudlibc_interceptors.h>

char *__cloudlibc_strrchr(const char *s, int c) {
  char *last = NULL;
  for (;;) {
    if (*s == (char)c)
      last = (char *)s;
    if (*s++ == '\0')
      return last;
  }
}

__weak_reference(__cloudlibc_strrchr, strrchr);
