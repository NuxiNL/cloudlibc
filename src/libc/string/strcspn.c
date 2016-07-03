// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/byteset.h>

#include <string.h>

size_t strcspn(const char *s1, const char *s2) {
  // Construct span bitmask.
  byteset_t bs;
  byteemptyset(&bs);
  do {
    byteaddset(&bs, *s2);
  } while (*s2++ != '\0');

  // Scan over input.
  const char *s = s1;
  while (!byteismember(&bs, *s))
    ++s;
  return s - s1;
}
