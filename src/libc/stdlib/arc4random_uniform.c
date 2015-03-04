// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdint.h>
#include <stdlib.h>

// Calculates a random number within the range [0, upper) without any
// modulo bias.
//
// The function below repeatedly obtains a random number from
// arc4random() until it lies within the range [2^32 % upper, 2^32). As
// this range has length k * upper, we can safely obtain a number
// without any modulo bias.
uint32_t arc4random_uniform(uint32_t upper) {
  // Compute 2^32 % upper
  //      == (2^32 - upper) % upper
  //      == -upper % upper.
  uint32_t lower = -upper % upper;
  for (;;) {
    uint32_t value = arc4random();
    if (value >= lower)
      return value % upper;
  }
}
