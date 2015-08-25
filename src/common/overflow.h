// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef COMMON_OVERFLOW_H
#define COMMON_OVERFLOW_H

#include <limits.h>
#include <stdbool.h>

// Implementations for types for which no intrinsics exist.
static inline bool uaddh_overflow(unsigned short a, unsigned short b,
                                  unsigned short *c) {
  *c = a + b;
  return a > USHRT_MAX - b;
}

static inline bool usubh_overflow(unsigned short a, unsigned short b,
                                  unsigned short *c) {
  *c = a - b;
  return a < b;
}

static inline bool umulh_overflow(unsigned short a, unsigned short b,
                                  unsigned short *c) {
  *c = a * b;
  return b != 0 && a > USHRT_MAX / b;
}

// clang-format off
#define COMPUTE_OVERFLOW(op, x, y, out)                 \
  _Generic(*(out),                                      \
           int: __builtin_s##op##_overflow,             \
           long: __builtin_s##op##l_overflow,           \
           long long: __builtin_s##op##ll_overflow,     \
           unsigned short: u##op##h_overflow,           \
           unsigned int: __builtin_u##op##_overflow,    \
           unsigned long: __builtin_u##op##l_overflow,  \
           unsigned long long: __builtin_u##op##ll_overflow)(x, y, out)
// clang-format on

// Performs an addition, subtraction or multiplication operation,
// returning whether the computation caused an overflow.
#define add_overflow(x, y, out) COMPUTE_OVERFLOW(add, x, y, out)
#define sub_overflow(x, y, out) COMPUTE_OVERFLOW(sub, x, y, out)
#define mul_overflow(x, y, out) COMPUTE_OVERFLOW(mul, x, y, out)

#endif
