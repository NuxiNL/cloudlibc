// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <stdlib.h>

uint32_t arc4random(void) {
  uint32_t value;
  arc4random_buf(&value, sizeof(value));
  return value;
}
