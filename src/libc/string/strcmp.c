// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "string_impl.h"

int strcmp(const char *sb1, const char *sb2) {
  // Special case: both strings start within the same byte of a long.
  // This allows us to compare multiple characters at a time.
  if (is_long_aligned_equally(sb1, sb2)) {
    // Do bytewise comparison until the strings are aligned to long.
    while (!is_long_aligned(sb1)) {
      unsigned char c1 = *sb1++;
      unsigned char c2 = *sb2++;
      if (c1 != c2)
        return (int)c1 - (int)c2;
      if (c1 == '\0')
        return 0;
    }

    // Compare using longs.
    const unsigned long *sl1 = (const unsigned long *)sb1;
    const unsigned long *sl2 = (const unsigned long *)sb2;
    while (*sl1 == *sl2 && !contains_nullbyte(*sl1)) {
      ++sl1;
      ++sl2;
    }
    sb1 = (const char *)sl1;
    sb2 = (const char *)sl2;
  }

  // Do bytewise comparison for remainder or unaligned strings.
  for (;;) {
    unsigned char c1 = *sb1++;
    unsigned char c2 = *sb2++;
    if (c1 != c2)
      return (int)c1 - (int)c2;
    if (c1 == '\0')
      return 0;
  }
}
