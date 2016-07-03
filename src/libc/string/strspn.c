// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/byteset.h>

#include <string.h>

size_t strspn(const char *s1, const char *s2) {
  // Construct span bitmask.
  byteset_t bs;
  byteemptyset(&bs);
  while (*s2 != '\0') {
    byteaddset(&bs, *s2);
    ++s2;
  }

  // Scan over input.
  const char *s = s1;
  while (byteismember(&bs, *s))
    ++s;
  return s - s1;
}
