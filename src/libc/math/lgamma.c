// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <math.h>

double lgamma(double x) {
  int signgam;
  return lgamma_r(x, &signgam);
}
