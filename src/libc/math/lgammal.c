// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <math.h>

long double lgammal(long double x) {
  int signgam;
  return lgammal_r(x, &signgam);
}
