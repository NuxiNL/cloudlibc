// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/float10.h>

#include <double-conversion.h>

using double_conversion::DoubleToStringConverter;
using double_conversion::StringBuilder;

static_assert(F10DEC_DIGITS_MAX ==
                  DoubleToStringConverter::kMaxExponentialDigits,
              "Value mismatch");

void __f10dec_exp(long double value, unsigned char *decimal,
                  unsigned char *digits, size_t ndigits, int *exponent) {
  assert(ndigits < F10DEC_DIGITS_MAX && "Number of digits out of bound");
  // TODO(edje): This should do a long double conversion instead.
  DoubleToStringConverter conv(
      DoubleToStringConverter::EMIT_TRAILING_DECIMAL_POINT, nullptr, nullptr,
      'e', 0, 0, 0, 0);
  char buf[DoubleToStringConverter::kMaxExponentialDigits + 9];
  StringBuilder sb(buf, sizeof(buf));
  if (conv.ToExponential(value, ndigits, &sb)) {
    // Conversion successful. Extract digits from output.
    const char *s = sb.Finalize();
    assert(*s >= '0' && *s <= '9' && "Invalid digit");
    *decimal = *s++ - '0';
    assert(*s == '.' && "Expected radix character");
    while (*++s != 'e') {
      assert(*s >= '0' && *s <= '9' && "Invalid digit");
      *digits++ = *s - '0';
      --ndigits;
    }
    assert(ndigits == 0 &&
           "ToExponential() returned the wrong number of digits");

    // Parse the exponent.
    assert(*s == 'e' && "Expected radix character");
    bool negative = *++s == '-';
    *exponent = 0;
    do {
      assert(*s >= '0' && *s <= '9' && "Invalid digit");
      *exponent = *exponent * 10 + *s - '0';
    } while (*++s != '\0');
    if (negative)
      *exponent = -*exponent;
  } else {
    // TODO(edje): This should abort, but right now this can happen
    // because of long doubles.
    memset(digits, '\0', ndigits);
    *exponent = -1;
  }
}
