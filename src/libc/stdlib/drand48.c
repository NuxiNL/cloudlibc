// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

double drand48(void) {
  // Obtain random data.
  uint64_t value;
  arc4random_buf(&value, sizeof(value));

  // Convert value to a floating point number between [0, 1).
  return ldexp(value, -64);
}
