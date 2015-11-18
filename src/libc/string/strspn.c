// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <limits.h>
#include <string.h>

#define INDEX(c) ((unsigned char)(c) / LONG_BIT)
#define BIT(c) (1UL << ((unsigned char)(c) % LONG_BIT))

size_t strspn(const char *s1, const char *s2) {
  // Construct span bitmask.
  unsigned long span[(UCHAR_MAX + 1) / LONG_BIT] = {};
  while (*s2 != '\0') {
    span[INDEX(*s2)] |= BIT(*s2);
    ++s2;
  }

  // Scan over input.
  const char *s = s1;
  while ((span[INDEX(*s)] & BIT(*s)) != 0)
    ++s;
  return s - s1;
}
