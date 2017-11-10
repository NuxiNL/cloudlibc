// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

char *(strrchr)(const char *s, int c) {
  char *last = NULL;
  for (;;) {
    if (*s == (char)c)
      last = (char *)s;
    if (*s++ == '\0')
      return last;
  }
}
