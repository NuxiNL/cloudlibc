// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef _STRING_STRING_IMPL_H_
#define _STRING_STRING_IMPL_H_

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

// Cutoff for when we consider a string to be long enough that it makes
// sense to optimize.
#define LONG_STRING_SIZE (4 * sizeof(unsigned long))

// Tests whether a pointer is aligned to unsigned long.
static inline bool is_long_aligned(const void *p) {
  return ((uintptr_t)p % sizeof(unsigned long)) == 0;
}

static inline bool is_long_aligned_equally(const void *a, const void *b) {
  return ((uintptr_t)a % sizeof(unsigned long)) ==
         ((uintptr_t)b % sizeof(unsigned long));
}

// Constructs a long consisting solely of bytes with a certain value.
static inline unsigned long construct_chars(unsigned char c) {
  unsigned long cl = c;
  for (int s = CHAR_BIT; s < LONG_BIT; s *= 2)
    cl |= cl << s;
  return cl;
}

// Tests whether the unsigned long contains null bytes. This function
// makes use of the following property:
//
//   ch == '\0' <=> (ch - 1 & ~ch & 0x80) != 0
//
// In other words, only for '\0' it holds that the top bit is clear,
// but set after decrementing.
static inline bool contains_nullbyte(unsigned long v) {
  return ((v - construct_chars(0x01)) & ~v & construct_chars(0x80)) != 0;
}

#endif
