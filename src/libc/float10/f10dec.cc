// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/float10.h>

#include <double-conversion/double-conversion.h>

#include <assert.h>
#include <fenv.h>
#include <limits.h>
#include <math.h>

using double_conversion::DoubleToStringConverter;

void __f10dec(long double value, unsigned int precision, unsigned char *digits,
              size_t *ndigits, int *exponent, int round) {
  // Convert the floating point value to the shortest sequence of
  // decimal digits that unambigously translates back to the original
  // value. Let's treat this representation as being exact.
  char buffer[DoubleToStringConverter::kBase10MaximalLength + 1];
  bool sign;
  int written;
  DoubleToStringConverter::DoubleToAscii(
      value, DoubleToStringConverter::SHORTEST, 0, buffer, sizeof(buffer),
      &sign, &written, exponent);

  for (;;) {
    if (written == 0) {
      // Floating point value zero.
      *exponent = 1;
      *ndigits = 0;
      return;
    }
    // Trim off trailing zero digits, as we don't need these.
    if (buffer[written - 1] != '0')
      break;
    --written;
  }

  // No way we can get more digits than the ones we obtained.
  if (*ndigits > (size_t)written)
    *ndigits = written;

  // Invert the rounding mode if the value is negative, so that the code
  // below does not need to take the sign bit into account.
  if (signbit(value)) {
    if (round == FE_UPWARD)
      round = FE_DOWNWARD;
    else if (round == FE_DOWNWARD)
      round = FE_UPWARD;
  }

  // Take the requested precision into account.
  assert(written > 0 && "Input value can no longer be zero");
  assert(*ndigits > 0 && "No buffer provided to store digits");
  if (precision <= INT_MAX) {
    if ((int)precision + *exponent <= 0) {
      // Number is smaller than what the precision allows to be shown.
      // Either return zero or a single one digit, depending on the
      // rounding mode.
      if (round == FE_UPWARD ||
          (round == FE_TONEAREST && (int)precision + *exponent == 0 &&
           buffer[0] >= '5')) {
        digits[0] = 1;
        *ndigits = 1;
        *exponent = 1 - (int)precision;
      } else {
        *ndigits = 0;
        *exponent = 1;
      }
      return;
    }

    // Number has more digits than the requested precision. Truncate.
    if ((int)*ndigits - *exponent > (int)precision)
      *ndigits = precision + *exponent;
  }

  // Round the full number to the number of requested digits according
  // to the provided rounding mode.
  if ((round == FE_UPWARD && *ndigits < (size_t)written) ||
      (round == FE_TONEAREST && buffer[*ndigits] >= '5')) {
    for (;;) {
      if (++buffer[*ndigits - 1] <= '9')
        break;
      if (--*ndigits == 0) {
        digits[0] = 1;
        *ndigits = 1;
        ++*exponent;
        return;
      }
    }
  }

  // Return remaining digits.
  for (size_t i = 0; i < *ndigits; ++i)
    digits[i] = buffer[i] - '0';
}
