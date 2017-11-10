// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

// Parser of floating point exponents, used by parser_strtofloat.h.

int exponent = 0;

if (PEEK_ISDIGIT(1) ||
    ((PEEK(1) == '-' || PEEK(1) == '+') && PEEK_ISDIGIT(2))) {
  // Skip 'e' or 'p' character.
  SKIP(1);

  // Optional plus or minus symbol.
  bool exp_negative = false;
  if (PEEK(0) == '-') {
    exp_negative = true;
    SKIP(1);
  } else if (PEEK(0) == '+') {
    SKIP(1);
  }

  // Parse the exponent value. Take care of overflows by stopping
  // processing if the exponent is already above a million. The
  // resulting floating point value would already overflow.
  while (PEEK(0) >= '0' && PEEK(0) <= '9') {
    if (exponent < 1000000)
      exponent = exponent * 10 + PEEK(0) - '0';
    SKIP(1);
  }
  if (exp_negative)
    exponent = -exponent;
}
