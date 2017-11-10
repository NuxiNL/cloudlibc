// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <math.h>

float lgammaf(float x) {
  int signgam;
  return lgammaf_r(x, &signgam);
}
