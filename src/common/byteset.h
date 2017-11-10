// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef COMMON_BYTESET_H
#define COMMON_BYTESET_H

#include <limits.h>
#include <stdbool.h>
#include <string.h>

// A simple set of byte values.
//
// In order to get a decent running time complexity, functions like
// strcspn(), strpbrk(), strspn() and strtok() need to keep track of a
// set that contains between 0 and 256 members, each corresponding with
// a certain byte value. The byteset_t type can be used for this
// purpose, having a similar API to sigset_t.

typedef struct {
  unsigned long bits[__howmany(UCHAR_MAX + 1, LONG_BIT)];
} byteset_t;

// Adds a byte to the set if not already present.
static inline void byteaddset(byteset_t *bs, unsigned char c) {
  bs->bits[c / LONG_BIT] |= 1UL << (c % LONG_BIT);
}

// Removes a byte from the set if present.
static inline void bytedelset(byteset_t *bs, unsigned char c) {
  bs->bits[c / LONG_BIT] &= ~(1UL << (c % LONG_BIT));
}

// Clears the set.
static inline void byteemptyset(byteset_t *bs) {
  memset(&bs->bits, 0, sizeof(bs->bits));
}

// Fills the set.
static inline void bytefillset(byteset_t *bs) {
  memset(&bs->bits, ~0, sizeof(bs->bits));
}

// Tests whether a byte is a member of the set.
static inline bool byteismember(byteset_t *bs, unsigned char c) {
  return (bs->bits[c / LONG_BIT] & (1UL << (c % LONG_BIT))) != 0;
}

#endif
