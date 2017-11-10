// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

// Parser of floating point literals as performed by strtod(), scanf(), etc.

#define PEEK_ISDIGIT(o) (PEEK(o) >= '0' && PEEK(o) <= '9')
#define PEEK_ISXDIGIT(o)                                                       \
  ((PEEK(o) >= '0' && PEEK(o) <= '9') || (PEEK(o) >= 'A' && PEEK(o) <= 'F') || \
   (PEEK(o) >= 'a' && PEEK(o) <= 'f'))

// Result of parsing.
bool have_number = false;
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
    have_number = true;
    SKIP(3);
    number = INFINITY;

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
    have_number = true;
    SKIP(3);
    number = NAN;

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
    // TODO(ed): The check above falsely assumes that '.' is the radix!
    // Hexadecimal floating point number.
    have_number = true;
    SKIP(2);
    struct f16enc f16;
    f16enc_init(&f16);
    int digits_exponent = 0;

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
      f16enc_push_xdigit(&f16, digit);
      SKIP(1);
    }

#include "parser_strtofloat_radixchar.h"
    if (parsed_radixchar) {
      // Parse digits after the period.
      for (;;) {
        uint_fast8_t digit;
        if (PEEK_ISDIGIT(0))
          digit = PEEK(0) - '0';
        else if (PEEK(0) >= 'A' && PEEK(0) <= 'F')
          digit = PEEK(0) - 'A' + 10;
        else if (PEEK(0) >= 'a' && PEEK(0) <= 'f')
          digit = PEEK(0) - 'a' + 10;
        else
          break;
        f16enc_push_xdigit(&f16, digit);
        digits_exponent -= 4;
        SKIP(1);
      }
    }

    // Exponentiation.
    if (PEEK(0) == 'p' || PEEK(0) == 'P') {
#include "parser_strtofloat_exponent.h"
      digits_exponent += exponent;
    }

    // Determine rounding. If we're going to generate a negative number,
    // invert FE_DOWNWARD and FE_UPWARD.
    int round = fegetround();
    if (negative) {
      if (round == FE_UPWARD)
        round = FE_DOWNWARD;
      else if (round == FE_DOWNWARD)
        round = FE_UPWARD;
    }

    // Generate floating point value.
    // clang-format off
    number = _Generic(
        number,
        float: f16enc_get_float,
        double: f16enc_get_double,
        long double: f16enc_get_long_double)(
            &f16, digits_exponent, round, &have_range_error);
    // clang-format on
  } else {
    // Decimal floating point number.
    char digits[DECIMAL_DIG];
    size_t ndigits = 0;
    int digits_exponent = 0;

    // Skip leading zeroes.
    while (PEEK(0) == '0') {
      have_number = true;
      SKIP(1);
    }

    // Parse digits before the period.
    while (PEEK_ISDIGIT(0)) {
      if (ndigits < sizeof(digits)) {
        // Enough space to store the digit.
        digits[ndigits++] = PEEK(0);
      } else {
        // Out of buffer space. Raise the exponent instead.
        ++digits_exponent;
      }
      have_number = true;
      SKIP(1);
    }

#include "parser_strtofloat_radixchar.h"
    if (parsed_radixchar) {
      // Parse digits after the period. Skip leading zeroes if we only
      // matched zeroes before the radix character.
      if (ndigits == 0) {
        while (PEEK(0) == '0') {
          have_number = true;
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
        have_number = true;
        SKIP(1);
      }
    }

    if (have_number) {
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
            float: __f10enc_get_float,
            double: __f10enc_get_double,
            long double: __f10enc_get_long_double)(
                digits, ndigits, digits_exponent);
        // clang-format on

        // The functions above can only be used to convert a finite
        // non-zero number. If they obtain infinity or zero, it means
        // that the conversion caused an overflow or underflow.
        switch (fpclassify(number)) {
          case FP_INFINITE:
          case FP_ZERO:
            have_range_error = true;
            break;
        }
      }
    }
  }
  if (negative)
    number = -number;
}

#undef PEEK_ISDIGIT
#undef PEEK_ISXDIGIT
