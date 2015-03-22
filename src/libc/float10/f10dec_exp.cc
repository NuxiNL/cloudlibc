// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/float10.h>

#include <double-conversion.h>

using double_conversion::DoubleToStringConverter;

void __f10dec_exp(long double value, unsigned char *digits, size_t *ndigits,
                  int *exponent) {
  char buffer[DoubleToStringConverter::kBase10MaximalLength + 1];
  bool sign;
  int written;
  DoubleToStringConverter::DoubleToAscii(
      value, DoubleToStringConverter::PRECISION, *ndigits, buffer,
      sizeof(buffer), &sign, &written, exponent);
  for (int i = 0; i < written; ++i)
    digits[i] = buffer[i] - '0';
  *ndigits = written;
}
