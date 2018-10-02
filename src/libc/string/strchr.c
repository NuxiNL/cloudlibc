// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>
#include <cloudlibc_interceptors.h>

char *__cloudlibc_strchr(const char *s, int c) {
  for (;;) {
    if (*s == (char)c)
      return (char *)s;
    if (*s++ == '\0')
      return NULL;
  }
}

__weak_reference(__cloudlibc_strchr, strchr);
