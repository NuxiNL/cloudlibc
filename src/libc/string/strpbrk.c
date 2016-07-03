// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/byteset.h>

#include <string.h>

char *(strpbrk)(const char *s1, const char *s2) {
  // Construct span bitmask.
  byteset_t bs;
  byteemptyset(&bs);
  do {
    byteaddset(&bs, *s2);
  } while (*s2++ != '\0');

  // Scan over input.
  while (!byteismember(&bs, *s1))
    ++s1;
  return *s1 != '\0' ? (char *)s1 : NULL;
}
