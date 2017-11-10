// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <stdlib.h>

int rand(void) {
  return arc4random_uniform((uint32_t)RAND_MAX + 1);
}
