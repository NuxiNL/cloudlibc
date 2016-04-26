// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>

void *(memrchr)(const void *s, int c, size_t n) {
  const unsigned char *sb = s;
  sb += n;
  while (n-- > 0) {
    --sb;
    if (*sb == (unsigned char)c)
      return (void *)sb;
  }
  return NULL;
}
