// Copyright (c) 2016-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <math.h>
#include <cloudlibc_interceptors.h>

float __cloudlibc_lgammaf(float x) {
  int signgam;
  return lgammaf_r(x, &signgam);
}

__weak_reference(__cloudlibc_lgammaf, lgammaf);
