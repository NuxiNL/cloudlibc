// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/float.h>

#include <strtod.h>

float __float10_to_float(const char *str, size_t len, int exponent) {
  return double_conversion::Strtof(
      double_conversion::Vector<const char>(str, len), exponent);
}
