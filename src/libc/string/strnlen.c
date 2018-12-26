// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <cloudlibc_interceptors.h>
#include <string.h>

#include "string_impl.h"

size_t __cloudlibc_strnlen(const char *s, size_t maxlen) {
  // Perform byte comparisons until the string is aligned to unsigned long.
  const char *eb = s;
  while (!is_long_aligned(eb)) {
    if (maxlen-- == 0 || *eb == '\0')
      return eb - s;
    ++eb;
  }

  // Skip words completely if they do not contain any null bytes.
  const unsigned long *el = (const unsigned long *)eb;
  while (maxlen >= sizeof(unsigned long) && !contains_nullbyte(*el)) {
    maxlen -= sizeof(unsigned long);
    ++el;
  }

  // Switch back to byte comparisons for the last couple of characters.
  eb = (const char *)el;
  while (maxlen-- > 0 && *eb != '\0')
    ++eb;
  return eb - s;
}

__weak_reference(__cloudlibc_strnlen, strnlen);
