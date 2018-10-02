// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/byteset.h>

#include <string.h>
#include <cloudlibc_interceptors.h>

size_t __cloudlibc_strcspn(const char *s1, const char *s2) {
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

__weak_reference(__cloudlibc_strcspn, strcspn);
