// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>

#include "string_impl.h"

void *memcpy(void *restrict s1, const void *restrict s2, size_t n) {
  unsigned char *sb1 = (unsigned char *)s1;
  const unsigned char *sb2 = (const unsigned char *)s2;

  // Special case: both buffers start within the same byte of a long.
  // This allows us to copy multiple bytes at a time.
  if (n >= LONG_STRING_SIZE && is_long_aligned_equally(sb1, sb2)) {
    // Do bytewise copying until the buffers are aligned to long.
    while (!is_long_aligned(sb1)) {
      *sb1++ = *sb2++;
      --n;
    }

    // Copy using longs.
    unsigned long *sl1 = (unsigned long *)sb1;
    const unsigned long *sl2 = (const unsigned long *)sb2;
    do {
      *sl1++ = *sl2++;
      n -= sizeof(unsigned long);
    } while (n >= sizeof(unsigned long));
    sb1 = (unsigned char *)sl1;
    sb2 = (const unsigned char *)sl2;
  }

  // Do bytewise copying for remainder or unaligned buffers.
  while (n-- > 0)
    *sb1++ = *sb2++;
  return s1;
}
