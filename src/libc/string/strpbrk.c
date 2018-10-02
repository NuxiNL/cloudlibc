// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/byteset.h>

#include <string.h>
#include <cloudlibc_interceptors.h>

char *__cloudlibc_strpbrk(const char *s1, const char *s2) {
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

__weak_reference(__cloudlibc_strpbrk, strpbrk);
