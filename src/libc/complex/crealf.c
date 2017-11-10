// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <complex.h>

float crealf(float complex z) {
  return __real__(z);
}
