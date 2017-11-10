// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <complex.h>

double cimag(double complex z) {
  return __imag__(z);
}
