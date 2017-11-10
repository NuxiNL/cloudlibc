// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/float10.h>

#include <double-conversion/strtod.h>

float __f10enc_get_float(const char *str, size_t len, int exponent) {
  return double_conversion::Strtof(
      double_conversion::Vector<const char>(str, len), exponent);
}
