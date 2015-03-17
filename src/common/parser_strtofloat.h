// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

// Parser of floating point literals as performed by strtod(), scanf(), etc.

// Result of parsing.
bool have_number = false;
flt_t number;

{
  bool negative = false;
  // Negative or positive number?
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
    have_number = true;

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
    have_number = true;

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
  } else if (PEEK(0) == '0' && (PEEK(1) == 'x' || PEEK(1) == 'X')) {
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
      have_number = true;

      if ((f2.significand[0] & 0xf000000000000000) == 0) {
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
      SKIP(1);
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
        have_number = true;

        if ((f2.significand[0] & 0xf000000000000000) == 0) {
          // Still space left to fit the digit. Decrease the exponent for
          // every digit seen.
          f2.significand[0] =
              (f2.significand[0] << 4) | (f2.significand[1] >> 60);
          f2.significand[1] = (f2.significand[1] << 4) | digit;
          f2.exponent -= 4;
        }
      }
    }

    if (have_number) {
      // Exponentiation.
      if ((PEEK(0) == 'p' || PEEK(0) == 'P') &&
          ((PEEK(1) >= '0' && PEEK(1) <= '9') ||
           ((PEEK(1) == '-' || PEEK(1) == '+') && PEEK(2) >= '0' &&
            PEEK(2) <= '9'))) {
        // TODO(edje): Bounds checking.
        bool exp_negative = PEEK(1) == '-';
        SKIP(2);
        int exponent = 0;
        while (PEEK(0) >= '0' && PEEK(0) <= '9') {
          exponent = exponent * 10 + PEEK(0) - '0';
          SKIP(1);
        }
        if (exp_negative)
          f2.exponent -= exponent;
        else
          f2.exponent += exponent;
      }

      // Turn into normalized base-2 floating point number.
      // TODO(edje): Error handling.
      __float2_normalize(&f2);
      _Generic(number, float
               : __float2_to_float, double
               : __float2_to_double, long double
               : __float2_to_long_double)(&f2, &number);
    }
  } else {
    // Decimal floating point number.
    // TODO(edje): Implement.

    // Parse digits before the period.
    while (PEEK(0) >= '0' && PEEK(0) <= '9') {
      // uint_fast8_t digit = PEEK(0) - '0';
      SKIP(1);
      have_number = true;
    }

    // Parse digits after the period.
    // TODO(edje): Use LC_NUMERIC radix character.
    if (PEEK(0) == '.') {
      SKIP(1);
      while (PEEK(0) >= '0' && PEEK(0) <= '9') {
        // uint_fast8_t digit = PEEK(0) - '0';
        SKIP(1);
        have_number = true;
      }
    }

    if (have_number) {
      // Exponentiation.
      if ((PEEK(0) == 'e' || PEEK(0) == 'E') &&
          ((PEEK(1) >= '0' && PEEK(1) <= '9') ||
           ((PEEK(1) == '-' || PEEK(1) == '+') && PEEK(2) >= '0' &&
            PEEK(2) <= '9'))) {
        // TODO(edje): Bounds checking.
        // bool negative = PEEK(1) == '-';
        SKIP(2);
        int exponent = 0;
        while (PEEK(0) >= '0' && PEEK(0) <= '9') {
          exponent = exponent * 10 + PEEK(0) - '0';
          SKIP(1);
        }
      }

      // TODO(edje): store/translate/normalize.
    }
  }
}
