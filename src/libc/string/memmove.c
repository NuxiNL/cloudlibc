// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>

void *memmove(void *s1, const void *s2, size_t n) {
  char *sb1 = s1;
  const char *sb2 = s2;
  if (sb1 < sb2) {
    while (n-- > 0)
      *sb1++ = *sb2++;
  } else if (sb1 > sb2) {
    sb1 += n;
    sb2 += n;
    while (n-- > 0)
      *--sb1 = *--sb2;
  }
  return s1;
}
