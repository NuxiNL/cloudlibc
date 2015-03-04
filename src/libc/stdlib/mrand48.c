// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdint.h>
#include <stdlib.h>

long mrand48(void) {
  int32_t value;
  arc4random_buf(&value, sizeof(value));
  return value;
}
