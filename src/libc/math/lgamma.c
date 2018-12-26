// Copyright (c) 2016-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <math.h>
#include <cloudlibc_interceptors.h>

double __cloudlibc_lgamma(double x) {
  int signgam;
  return lgamma_r(x, &signgam);
}

__weak_reference(__cloudlibc_lgamma, lgamma);
