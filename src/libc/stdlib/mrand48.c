// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <stdlib.h>

long mrand48(void) {
  int32_t value;
  arc4random_buf(&value, sizeof(value));
  return value;
}
