// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdint.h>
#include <stdlib.h>

#include "stdlib_impl.h"

long nrand48(uint16_t *xsubi) {
  return lcg48(xsubi) >> 17;
}
