// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

// Parser of floating point literals as performed by strtod(), scanf(), etc.

// Result of parsing.
bool have_number = false;
struct float2 number = {};

// Negative or positive number?
if (PEEK(0) == '-') {
  number.flags |= F2_NEGATIVE;
  SKIP(1);
} else if (PEEK(0) == '+') {
  SKIP(1);
}

if ((PEEK(0) == 'i' || PEEK(0) == 'I') && (PEEK(1) == 'n' || PEEK(1) == 'N') &&
    (PEEK(2) == 'f' || PEEK(2) == 'F')) {
  // INF.
  SKIP(3);
  number.flags |= F2_INFINITE;
  have_number = true;

  // INFINITY.
  if ((PEEK(0) == 'i' || PEEK(0) == 'I') &&
      (PEEK(1) == 'n' || PEEK(1) == 'N') &&
      (PEEK(2) == 'i' || PEEK(2) == 'I') &&
      (PEEK(3) == 't' || PEEK(3) == 'T') && (PEEK(4) == 'y' || PEEK(4) == 'Y'))
    SKIP(5);
} else if ((PEEK(0) == 'n' || PEEK(0) == 'N') &&
           (PEEK(1) == 'a' || PEEK(1) == 'A') &&
           (PEEK(2) == 'n' || PEEK(2) == 'N')) {
  // NAN.
  SKIP(3);
  number.flags |= F2_NAN;
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
  number.exponent = 0;

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

    if ((number.significand[0] & 0xf000000000000000) == 0) {
      // Still space left to fit the digit.
      number.significand[0] =
          (number.significand[0] << 4) | (number.significand[1] >> 60);
      number.significand[1] = (number.significand[1] << 4) | digit;
    } else {
      // No space left. Just increase the exponent instead.
      number.exponent += 4;
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

      if ((number.significand[0] & 0xf000000000000000) == 0) {
        // Still space left to fit the digit. Decrease the exponent for
        // every digit seen.
        number.significand[0] =
            (number.significand[0] << 4) | (number.significand[1] >> 60);
        number.significand[1] = (number.significand[1] << 4) | digit;
        number.exponent -= 4;
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
      bool negative = PEEK(1) == '-';
      SKIP(2);
      int exponent = 0;
      while (PEEK(0) >= '0' && PEEK(0) <= '9') {
        exponent = exponent * 10 + PEEK(0) - '0';
        SKIP(1);
      }
      if (negative)
        number.exponent -= exponent;
      else
        number.exponent += exponent;
    }

    // Turn into normalized base-2 floating point number.
    __float2_normalize(&number);
  }
} else {
  // Decimal floating point number.
  number.exponent = 0;

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
