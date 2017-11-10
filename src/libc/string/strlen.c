// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "string_impl.h"

size_t strlen(const char *s) {
  // Perform byte comparisons until the string is aligned to unsigned long.
  const char *eb = s;
  while (!is_long_aligned(eb)) {
    if (*eb == '\0')
      return eb - s;
    ++eb;
  }

  // Skip words completely if they do not contain any null bytes.
  const unsigned long *el = (const unsigned long *)eb;
  while (!contains_nullbyte(*el))
    ++el;

  // Switch back to byte comparisons for the last couple of characters.
  eb = (const char *)el;
  while (*eb != '\0')
    ++eb;
  return eb - s;
}
