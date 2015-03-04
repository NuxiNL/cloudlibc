// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <float.h>
#include <stdint.h>
#include <stdlib.h>

double drand48(void) {
#if DBL_MANT_DIG == 53
  // Obtain random data.
  uint64_t value;
  arc4random_buf(&value, sizeof(value));

  // Convert value to a floating point number between [1, 2).
  union {
    uint64_t i;
    double f;
  } conv = {.i = UINT64_C(0x3ff0000000000000) |
                 (value & UINT64_C(0xfffffffffffff))};

  // Bring value in range.
  return conv.f - 1.0;
#else
#error "Unknown floating point format"
#endif
}
