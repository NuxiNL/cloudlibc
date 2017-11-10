// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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
