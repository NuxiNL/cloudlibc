// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "string_impl.h"

int memcmp(const void *s1, const void *s2, size_t n) {
  const unsigned char *sb1 = (const unsigned char *)s1;
  const unsigned char *sb2 = (const unsigned char *)s2;

  // Special case: both buffers start within the same byte of a long.
  // This allows us to compare multiple characters at a time.
  if (n >= LONG_STRING_SIZE && is_long_aligned_equally(sb1, sb2)) {
    // Do bytewise comparison until the buffers are aligned to long.
    while (!is_long_aligned(sb1)) {
      unsigned char c1 = *sb1++;
      unsigned char c2 = *sb2++;
      if (c1 != c2)
        return (int)c1 - (int)c2;
      --n;
    }

    // Compare using longs.
    const unsigned long *sl1 = (const unsigned long *)sb1;
    const unsigned long *sl2 = (const unsigned long *)sb2;
    while (n >= sizeof(unsigned long) && *sl1 == *sl2) {
      ++sl1;
      ++sl2;
      n -= sizeof(unsigned long);
    }
    sb1 = (const unsigned char *)sl1;
    sb2 = (const unsigned char *)sl2;
  }

  // Do bytewise comparison for remainder or unaligned buffers.
  while (n-- > 0) {
    unsigned char c1 = *sb1++;
    unsigned char c2 = *sb2++;
    if (c1 != c2)
      return (int)c1 - (int)c2;
  }
  return 0;
}
