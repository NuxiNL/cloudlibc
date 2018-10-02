// Copyright (c) 2016-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <math.h>
#include <cloudlibc_interceptors.h>

long double __cloudlibc_lgammal(long double x) {
  int signgam;
  return lgammal_r(x, &signgam);
}

__weak_reference(__cloudlibc_lgammal, lgammal);
