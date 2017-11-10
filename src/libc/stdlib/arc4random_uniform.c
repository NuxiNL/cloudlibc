// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <stdlib.h>

// Calculates a random number within the range [0, upper - 1] without
// any modulo bias.
//
// The function below repeatedly obtains a random number from
// arc4random() until it lies within the range [2^32 % upper, 2^32). As
// this range has length k * upper, we can safely obtain a number
// without any modulo bias.
uint32_t arc4random_uniform(uint32_t upper) {
  // Corner case: obtain a random number in the range
  // [0, 0 - 1] == [0, UINT32_MAX].
  if (upper == 0) {
    uint32_t value;
    arc4random_buf(&value, sizeof(value));
    return value;
  }

  // Compute 2^32 % upper
  //      == (2^32 - upper) % upper
  //      == -upper % upper.
  uint32_t lower = -upper % upper;
  for (;;) {
    uint32_t value;
    arc4random_buf(&value, sizeof(value));
    if (value >= lower)
      return value % upper;
  }
}
