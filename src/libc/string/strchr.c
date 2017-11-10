// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

char *(strchr)(const char *s, int c) {
  for (;;) {
    if (*s == (char)c)
      return (char *)s;
    if (*s++ == '\0')
      return NULL;
  }
}
