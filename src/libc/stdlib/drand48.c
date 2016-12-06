// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

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
