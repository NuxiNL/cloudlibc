// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

size_t strlcpy(char *restrict s1, const char *restrict s2, size_t n) {
  // Copy up to n - 1 characters into the destination buffer.
  const char *begin = s2;
  while (n > 1) {
    *s1 = *s2;
    if (*s2 == '\0')
      return s2 - begin;
    ++s1;
    ++s2;
    --n;
  }

  // Nul-terminate the destination buffer if space is available.
  if (n > 0)
    *s1 = '\0';

  // Continue computing the length of s2.
  while (*s2 != '\0')
    ++s2;
  return s2 - begin;
}
