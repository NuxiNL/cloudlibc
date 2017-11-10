// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <complex.h>

float cimagf(float complex z) {
  return __imag__(z);
}
