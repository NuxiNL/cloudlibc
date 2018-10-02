// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/byteset.h>

#include <string.h>
#include <cloudlibc_interceptors.h>

size_t __cloudlibc_strspn(const char *s1, const char *s2) {
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

__weak_reference(__cloudlibc_strspn, strspn);
