// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>

void *memccpy(void *restrict s1, const void *restrict s2, int c, size_t n) {
  unsigned char *sb1 = s1;
  const unsigned char *sb2 = s2;
  while (n-- > 0) {
    *sb1 = *sb2++;
    if (*sb1++ == (unsigned char)c)
      return sb1;
  }
  return NULL;
}
