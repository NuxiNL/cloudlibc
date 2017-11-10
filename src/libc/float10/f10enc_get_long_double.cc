// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/float10.h>

#include <double-conversion/strtod.h>

long double __f10enc_get_long_double(const char *str, size_t len,
                                     int exponent) {
  // TODO(ed): double-conversion does not support long double.
  return double_conversion::Strtod(
      double_conversion::Vector<const char>(str, len), exponent);
}
