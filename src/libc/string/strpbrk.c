// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <limits.h>
#include <string.h>

#define INDEX(c) ((unsigned char)(c) / LONG_BIT)
#define BIT(c) (1UL << ((unsigned char)(c) % LONG_BIT))

char *strpbrk(const char *s1, const char *s2) {
  // Construct span bitmask.
  unsigned long span[(UCHAR_MAX + 1) / LONG_BIT] = {};
  do {
    span[INDEX(*s2)] |= BIT(*s2);
  } while (*s2++ != '\0');

  // Scan over input.
  while ((span[INDEX(*s1)] & BIT(*s1)) == 0)
    ++s1;
  return *s1 != '\0' ? (char *)s1 : NULL;
}
