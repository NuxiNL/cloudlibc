// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/float10.h>
#include <common/locale.h>

#include <assert.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>

#if WIDE
#include <wchar.h>
typedef wchar_t char_t;
#else
#include <monetary.h>
typedef char char_t;
#endif

ssize_t NAME(char_t *restrict s, size_t maxsize, locale_t locale,
             const char_t *restrict format, va_list ap) {
  const struct lc_monetary *monetary = locale->monetary;
  size_t nwritten = 0;
#define PUTCHAR(c)             \
  do {                         \
    if (nwritten >= maxsize) { \
      errno = E2BIG;           \
      return -1;               \
    }                          \
    s[nwritten++] = (c);       \
  } while (0)

  while (*format != '\0') {
    if (*format == '%') {
      ++format;

      // Parse flags.
      char_t fill_character = ' ';
      bool grouping = true, parentheses = false, currency_symbol = true,
           left_justified = false;
      for (;;) {
        if (*format == '=') {
          fill_character = *++format;
        } else if (*format == '^') {
          grouping = false;
        } else if (*format == '+') {
          parentheses = false;
        } else if (*format == '(') {
          parentheses = true;
        } else if (*format == '!') {
          currency_symbol = false;
        } else if (*format == '-') {
          left_justified = true;
        } else {
          break;
        }
        ++format;
      }

      // Field width.
      unsigned int field_width = 0;
      while (*format >= '0' && *format <= '9')
        field_width = field_width * 10 + *format++ - '0';

      // Left precision.
      unsigned int left_precision = 0;
      if (*format == '#') {
        ++format;
        while (*format >= '0' && *format <= '9')
          left_precision = left_precision * 10 + *format++ - '0';
      }

      // Right precision.
      unsigned int right_precision;
      if (*format == '.') {
        ++format;
        right_precision = 0;
        while (*format >= '0' && *format <= '9')
          right_precision = right_precision * 10 + *format++ - '0';
      } else {
        // TODO(edje): Get default value from locale.
        right_precision = 2;
      }

      if (*format == 'i' || *format == 'n') {
        // Convert floating point value to decimal digits.
        double value = va_arg(ap, double);
        bool negative = signbit(value);
        unsigned char digits[DECIMAL_DIG];
        size_t ndigits = sizeof(digits);
        int exponent;
        __f10dec_fixed(value, right_precision, digits, &ndigits, &exponent);

        // Values obtained from the locale.
        // TODO(edje): Set!
        size_t thousands_sep_width = 1;

        // Determine the number of characters printed before the decimal point.
        struct numeric_grouping numeric_grouping;
        size_t left_digits_with_grouping = exponent >= 1 ? exponent : 1;
        left_digits_with_grouping +=
            numeric_grouping_init(&numeric_grouping, monetary->mon_grouping,
                                  left_digits_with_grouping) *
            thousands_sep_width;
        size_t left_precision_with_grouping =
            left_precision +
            numeric_grouping_init(&(struct numeric_grouping){},
                                  monetary->mon_grouping, left_precision) *
                thousands_sep_width;

        if (negative)
          PUTCHAR('-');
        PUTCHAR('$');

        ssize_t position = -exponent;
        ssize_t idx;
        if (exponent >= 1) {
          // At least one digit is placed before the radix character.
          position = exponent;
          idx = 0;
        } else {
          // None of the digits are placed before the radix character.
          // Force zero padding.
          position = 1;
          idx = -exponent - 1;
        }

        // Print fill characters at the start of the value.
        while (left_precision_with_grouping-- > left_digits_with_grouping)
          PUTCHAR(fill_character);

        // Print digits from the value.
        while (position > -(ssize_t)right_precision) {
          unsigned char digit =
              idx >= 0 && (size_t)idx < ndigits ? digits[idx] : 0;
          if (position > 0) {
            // Print the grouping character.
            if (numeric_grouping_step(&numeric_grouping))
              PUTCHAR(',');
          } else if (position == 0) {
            // Print the radix character.
            PUTCHAR('.');
          }
          PUTCHAR(digit + '0');
          --position;
          ++idx;
        }

        assert(idx >= (ssize_t)ndigits && "Not all digits have been printed");
      } else if (*format == '%') {
        PUTCHAR('%');
      }
    } else {
      PUTCHAR(*format);
    }
    ++format;
  }

  // Add trailing null byte.
  PUTCHAR('\0');
  return nwritten - 1;
}
