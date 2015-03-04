// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/float.h>

#define EXPONENT_DELTA (16383 + 127)

void __long_double_to_float2(long double f, struct float2 *f2) {
#ifdef __x86_64__
  union {
    struct {
      uint64_t significand;
      uint16_t exponent;
    };
    long double f;
  } value = {.f = f};
  *f2 = (struct float2){};

  if ((value.exponent & 0x8000) != 0) {
    // Set sign bit.
    f2->flags |= F2_NEGATIVE;
  }

  int exponent = value.exponent & 0x7fff;
  if (exponent == 0) {
    // Subnormal value. Exponent is treated as if it is 1.
    f2->significand[0] = value.significand;
    f2->exponent = 1 - EXPONENT_DELTA;
    __float2_normalize(f2);
  } else if (exponent == 0x7fff) {
    // NAN or infinity, depending on whether the significand is zero.
    f2->flags |= value.significand != 0 ? F2_NAN : F2_INFINITE;
  } else {
    // We should see a normal value. To prevent us from crashing in case
    // of a malformed floating point number, set the top bit to make it
    // normal.
    f2->significand[0] = 0x8000000000000000 | value.significand;
    f2->exponent = exponent - EXPONENT_DELTA;
  }
#else
#error "Not implemented"
#endif
}
