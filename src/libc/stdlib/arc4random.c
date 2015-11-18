// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdint.h>
#include <stdlib.h>

uint32_t arc4random(void) {
  uint32_t value;
  arc4random_buf(&value, sizeof(value));
  return value;
}
