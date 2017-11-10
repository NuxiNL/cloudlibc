// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "string_impl.h"

void *memmove(void *s1, const void *s2, size_t n) {
  unsigned char *sb1 = s1;
  const unsigned char *sb2 = s2;
  if (sb1 < sb2) {
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
  } else if (sb1 > sb2) {
    sb1 += n;
    sb2 += n;

    // Special case: both buffers start within the same byte of a long.
    // This allows us to copy multiple bytes at a time.
    if (n >= LONG_STRING_SIZE && is_long_aligned_equally(sb1, sb2)) {
      // Do bytewise copying until the buffers are aligned to long.
      while (!is_long_aligned(sb1)) {
        *--sb1 = *--sb2;
        --n;
      }

      // Copy using longs.
      unsigned long *sl1 = (unsigned long *)sb1;
      const unsigned long *sl2 = (const unsigned long *)sb2;
      do {
        *--sl1 = *--sl2;
        n -= sizeof(unsigned long);
      } while (n >= sizeof(unsigned long));
      sb1 = (unsigned char *)sl1;
      sb2 = (const unsigned char *)sl2;
    }

    // Do bytewise copying for remainder or unaligned buffers.
    while (n-- > 0)
      *--sb1 = *--sb2;
  }
  return s1;
}

__strong_reference(memmove, memcpy);

#ifdef __arm__
__strong_reference(memmove, __aeabi_memcpy);
__strong_reference(memmove, __aeabi_memcpy4);
__strong_reference(memmove, __aeabi_memcpy8);
__strong_reference(memmove, __aeabi_memmove);
__strong_reference(memmove, __aeabi_memmove4);
__strong_reference(memmove, __aeabi_memmove8);
#endif
