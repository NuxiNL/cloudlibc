// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include "stdlib_impl.h"

double erand48(uint16_t *xsubi) {
  return ldexp(lcg48(xsubi), -48);
}
