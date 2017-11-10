// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <stdlib.h>

#include "stdlib_impl.h"

long nrand48(uint16_t *xsubi) {
  return lcg48(xsubi) >> 17;
}
