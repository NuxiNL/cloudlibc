// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/float.h>

#include <assert.h>
#include <float.h>
#include <math.h>

#define SIGNIFICAND_BITS 23
#define EXPONENT_DELTA (127 + 127)
#define EXPONENT_MASK (((uint32_t)1 << SIGNIFICAND_BITS) - 1)

bool __float2_to_float(const struct float2 *f2, float *f) {
  union {
    uint32_t i;
    float f;
  } value = {.i = 0};

  if ((f2->flags & F2_NEGATIVE) != 0) {
    // Set sign bit.
    value.i |= 0x80000000;
  }

  if ((f2->flags & F2_INFINITE) != 0) {
    // Set all the bits in the exponent and leave the significand zero.
    value.i |= 0x7f800000;
  } else if ((f2->flags & F2_NAN) != 0) {
    // Set all the bits in the exponent. Also make sure the significand
    // has a non-zero value.
    value.i |= 0x7fc00000;
  } else if (f2->significand[0] == 0) {
    // Significand has value 0. Nothing left to do.
    assert(f2->significand[1] == 0 && "Floating point is not normalized");
  } else if (f2->exponent < 1 - EXPONENT_DELTA) {
    // Underflow. Set value to FLT_MIN.
    // TODO(edje): Add support for subnormal floats?
    value.i |= 0x00800000;
  } else if (f2->exponent > 254 - EXPONENT_DELTA) {
    // Overflow. Set value to HUGE_VAL (== INFINITY).
    value.i |= 0x7f800000;
  } else {
    assert((f2->significand[0] & 0x8000000000000000) != 0 &&
           "Floating point is not normalized");
    // TODO(edje): Take rounding mode into account.
    value.i |= ((uint32_t)(f2->significand[0] >> (63 - SIGNIFICAND_BITS)) &
                EXPONENT_MASK) |
               ((uint32_t)(f2->exponent + EXPONENT_DELTA) << SIGNIFICAND_BITS);
  }
  *f = value.f;
  return true;
}
