// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <stdlib.h>

#include "stdlib_impl.h"

long jrand48(uint16_t *xsubi) {
  return (int32_t)(lcg48(xsubi) >> 16);
}
