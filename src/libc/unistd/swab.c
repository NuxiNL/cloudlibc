// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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
