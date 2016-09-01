// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <math.h>

long double lgammal(long double x) {
  int signgam;
  return lgammal_r(x, &signgam);
}
