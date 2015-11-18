// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <unistd.h>

void swab(const void *restrict src, void *restrict dest, ssize_t nbytes) {
  const char *s = src;
  char *d = dest;
  while (nbytes >= 2) {
    char tmp = *s++;
    *d++ = *s++;
    *d++ = tmp;
    nbytes -= 2;
  }
}
