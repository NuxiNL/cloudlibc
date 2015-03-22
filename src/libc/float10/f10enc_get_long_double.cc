// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/float10.h>

#include <strtod.h>

long double __f10enc_get_long_double(const char *str, size_t len,
                                     int exponent) {
  // TODO(edje): double-conversion does not support long double.
  return double_conversion::Strtod(
      double_conversion::Vector<const char>(str, len), exponent);
}
