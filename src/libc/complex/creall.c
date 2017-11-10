// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <complex.h>

long double creall(long double complex z) {
  return __real__(z);
}
