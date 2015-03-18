// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

// Parser of floating point literals as performed by strtod(), scanf(), etc.

#define PEEK_ISDIGIT(o) (PEEK(o) >= '0' && PEEK(o) <= '9')
#define PEEK_ISXDIGIT(o)                                                       \
  ((PEEK(o) >= '0' && PEEK(o) <= '9') || (PEEK(o) >= 'A' && PEEK(o) <= 'F') || \
   (PEEK(o) >= 'a' && PEEK(o) <= 'f'))

// Result of parsing.
bool have_number = true;
bool have_range_error = false;
flt_t number;

{
  // Negative or positive number?
  bool negative = false;
  if (PEEK(0) == '-') {
    negative = true;
    SKIP(1);
  } else if (PEEK(0) == '+') {
    SKIP(1);
  }

  if ((PEEK(0) == 'i' || PEEK(0) == 'I') &&
      (PEEK(1) == 'n' || PEEK(1) == 'N') &&
      (PEEK(2) == 'f' || PEEK(2) == 'F')) {
    // INF.
    SKIP(3);
    number = negative ? -INFINITY : INFINITY;

    // INFINITY.
    if ((PEEK(0) == 'i' || PEEK(0) == 'I') &&
        (PEEK(1) == 'n' || PEEK(1) == 'N') &&
        (PEEK(2) == 'i' || PEEK(2) == 'I') &&
        (PEEK(3) == 't' || PEEK(3) == 'T') &&
        (PEEK(4) == 'y' || PEEK(4) == 'Y'))
      SKIP(5);
  } else if ((PEEK(0) == 'n' || PEEK(0) == 'N') &&
             (PEEK(1) == 'a' || PEEK(1) == 'A') &&
             (PEEK(2) == 'n' || PEEK(2) == 'N')) {
    // NAN.
    SKIP(3);
    number = negative ? -NAN : NAN;

    // NAN(...). Find closing parenthesis.
    if (PEEK(0) == '(') {
      size_t close = 1;
      while (PEEK(close) != '\0') {
        if (PEEK(close) == ')') {
          SKIP(close + 1);
          break;
        }
        ++close;
      }
    }
  } else if (PEEK(0) == '0' && (PEEK(1) == 'x' || PEEK(1) == 'X') &&
             (PEEK_ISXDIGIT(2) || (PEEK(2) == '.' && PEEK_ISXDIGIT(3)))) {
    // Hexadecimal floating point number.
    SKIP(2);
    struct float2 f2 = {
        .flags = negative ? F2_NEGATIVE : 0,
    };
    f2.exponent = 0;

    // Parse digits before the period.
    for (;;) {
      uint_fast8_t digit;
      if (PEEK(0) >= '0' && PEEK(0) <= '9')
        digit = PEEK(0) - '0';
      else if (PEEK(0) >= 'A' && PEEK(0) <= 'F')
        digit = PEEK(0) - 'A' + 10;
      else if (PEEK(0) >= 'a' && PEEK(0) <= 'f')
        digit = PEEK(0) - 'a' + 10;
      else
        break;
      SKIP(1);

      if (f2.significand[0] >> 60 == 0) {
        // Still space left to fit the digit.
        f2.significand[0] =
            (f2.significand[0] << 4) | (f2.significand[1] >> 60);
        f2.significand[1] = (f2.significand[1] << 4) | digit;
      } else {
        // No space left. Just increase the exponent instead.
        f2.exponent += 4;
      }
    }

    // Parse digits after the period.
    // TODO(edje): Use LC_NUMERIC radix character.
    if (PEEK(0) == '.') {
      for (;;) {
        SKIP(1);
        uint_fast8_t digit;
        if (PEEK_ISDIGIT(0))
          digit = PEEK(0) - '0';
        else if (PEEK(0) >= 'A' && PEEK(0) <= 'F')
          digit = PEEK(0) - 'A' + 10;
        else if (PEEK(0) >= 'a' && PEEK(0) <= 'f')
          digit = PEEK(0) - 'a' + 10;
        else
          break;

        if (f2.significand[0] >> 60 == 0) {
          // Still space left to fit the digit. Decrease the exponent for
          // every digit seen.
          f2.significand[0] =
              (f2.significand[0] << 4) | (f2.significand[1] >> 60);
          f2.significand[1] = (f2.significand[1] << 4) | digit;
          f2.exponent -= 4;
        }
      }
    }

    // Exponentiation.
    if (PEEK(0) == 'p' || PEEK(0) == 'P') {
#include "parser_strtofloat_exponent.h"
      f2.exponent += exponent;
    }

    // Turn into normalized base-2 floating point number.
    __float2_normalize(&f2);
    // clang-format off
    have_range_error = !_Generic(
        number,
        float: __float2_to_float,
        double: __float2_to_double,
        long double: __float2_to_long_double)(&f2, &number);
    // clang-format on
  } else if (PEEK_ISDIGIT(0) || (PEEK(0) == '.' && PEEK_ISDIGIT(1))) {
    // Decimal floating point number.
    char digits[DECIMAL_DIG];
    size_t ndigits = 0;
    int digits_exponent = 0;

    // Skip leading zeroes.
    while (PEEK(0) == '0')
      SKIP(1);

    // Parse digits before the period.
    while (PEEK_ISDIGIT(0)) {
      if (ndigits < sizeof(digits)) {
        // Enough space to store the digit.
        digits[ndigits++] = PEEK(0);
      } else {
        // Out of buffer space. Raise the exponent instead.
        ++digits_exponent;
      }
      SKIP(1);
    }

    // Parse digits after the period.
    // TODO(edje): Use LC_NUMERIC radix character.
    if (PEEK(0) == '.') {
      SKIP(1);
      // Skip leading zeroes if we only matched zeroes before the radix
      // character.
      if (ndigits == 0) {
        while (PEEK(0) == '0') {
          SKIP(1);
          --digits_exponent;
        }
      }
      while (PEEK_ISDIGIT(0)) {
        if (ndigits < sizeof(digits)) {
          // Enough space to store the digit.
          digits[ndigits++] = PEEK(0);
          --digits_exponent;
        }
        SKIP(1);
      }
    }

    // Exponentiation.
    if (PEEK(0) == 'e' || PEEK(0) == 'E') {
#include "parser_strtofloat_exponent.h"
      digits_exponent += exponent;
    }

    if (ndigits == 0) {
      number = 0.0;
    } else {
      // clang-format off
      number = _Generic(number,
          float: __float10_to_float,
          double: __float10_to_double,
          long double: __float10_to_long_double)(
              digits, ndigits, digits_exponent);
      // clang-format on

      // Overflow or underflow.
      switch (fpclassify(number)) {
        case FP_INFINITE:
        case FP_ZERO:
          have_range_error = true;
          break;
      }
    }
    if (negative)
      number = -number;
  } else {
    // Unknown format.
    have_number = false;
  }
}

#undef PEEK_ISDIGIT
#undef PEEK_ISXDIGIT
