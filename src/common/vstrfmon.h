// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/float10.h>
#include <common/locale.h>
#include <common/numeric_grouping.h>

#include <assert.h>
#include <errno.h>
#include <fenv.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <wchar.h>

#if WIDE
typedef wchar_t char_t;
#else
#include <monetary.h>
typedef char char_t;
#endif

#define PUTCHAR(c)         \
  do {                     \
    if (buflen-- == 0)     \
      return nwritten;     \
    buf[nwritten++] = (c); \
  } while (0)
#if WIDE
#define PUTSTRING(str)        \
  do {                        \
    const wchar_t *s = (str); \
    while (*s != L'\0') {     \
      if (buflen-- == 0)      \
        return nwritten;      \
      buf[nwritten++] = *s++; \
    }                         \
  } while (0)
#else
#define PUTSTRING(str)                                                 \
  do {                                                                 \
    const wchar_t *s = (str);                                          \
    const struct lc_ctype *ctype = locale->ctype;                      \
    while (*s != L'\0') {                                              \
      if (buflen < MB_LEN_MAX)                                         \
        return nwritten;                                               \
      ssize_t len = ctype->c32tomb(&buf[nwritten], *s++, ctype->data); \
      if (len < 0) {                                                   \
        buf[nwritten++] = '?';                                         \
        --buflen;                                                      \
      } else {                                                         \
        nwritten += len;                                               \
        buflen -= len;                                                 \
      }                                                                \
    }                                                                  \
  } while (0)
#endif

static size_t generate_prefix(char_t *buf, size_t buflen,
                              const wchar_t *currency_symbol, char cs_precedes,
                              const wchar_t *sign, char sign_posn,
                              bool negative, char separator, char sep_by_space,
                              locale_t locale) {
  size_t nwritten = 0;
  // Leading '(' if negative.
  if (sign_posn == 0 && negative)
    PUTCHAR('(');

  if (cs_precedes == 1) {
    // Currency symbol precedes the value.
    if (sign_posn == 1 || sign_posn == 3) {
      // Sign string precedes the currency symbol.
      PUTSTRING(sign);
      if (sep_by_space == 2)
        PUTCHAR(' ');
    }
    if (*currency_symbol != L'\0') {
      PUTSTRING(currency_symbol);
      if (sign_posn == 4) {
        // Sign string succeeds the currency symbol.
        if (sep_by_space == 2)
          PUTCHAR(separator);
        PUTSTRING(sign);
        if (sep_by_space == 1)
          PUTCHAR(' ');
      } else if (sep_by_space == 1) {
        PUTCHAR(separator);
      }
    }
  } else if (sign_posn == 1) {
    // Just the sign string.
    PUTSTRING(sign);
    if (sep_by_space == 2)
      PUTCHAR(' ');
  }
  return nwritten;
}

static size_t generate_suffix(char_t *buf, size_t buflen,
                              const wchar_t *currency_symbol, char cs_precedes,
                              const wchar_t *sign, char sign_posn,
                              bool negative, char separator, char sep_by_space,
                              locale_t locale) {
  size_t nwritten = 0;
  if (cs_precedes == 0) {
    // Currency symbol succeeds the value.
    if (sign_posn == 3) {
      // Sign string precedes the currency symbol.
      if (sep_by_space == 1)
        PUTCHAR(' ');
      PUTSTRING(sign);
    }
    if (*currency_symbol != L'\0') {
      if ((sign_posn == 3 && sep_by_space == 2) ||
          (sep_by_space == 1 && (sign_posn == 0 || sign_posn == 1 ||
                                 sign_posn == 2 || sign_posn == 4)))
        PUTCHAR(separator);
      PUTSTRING(currency_symbol);
      if (sign_posn == 2 || sign_posn == 4) {
        // Sign string succeeds the currency symbol.
        if (sep_by_space == 2)
          PUTCHAR(' ');
        PUTSTRING(sign);
      }
    }
  } else if (sign_posn == 2) {
    // Just the sign string.
    if (sep_by_space == 2)
      PUTCHAR(' ');
    PUTSTRING(sign);
  }

  // Trailing ')' if negative.
  if (sign_posn == 0 && negative)
    PUTCHAR(')');
  return nwritten;
}

#undef PUTCHAR
#undef PUTSTRING

ssize_t NAME(char_t *restrict s, size_t maxsize, locale_t locale,
             const char_t *restrict format, va_list ap) {
  // Output buffer handling.
  size_t nwritten = 0;
#define PUTCHAR(c)             \
  do {                         \
    if (nwritten >= maxsize) { \
      errno = E2BIG;           \
      return -1;               \
    }                          \
    s[nwritten++] = (c);       \
  } while (0)

  // Fetch attributes from the locale that are independent from the
  // flags and the values that are printed. Already convert the
  // thousands separator and the decimal point, for the reason that
  // their width strongly influences the width computation.
  const struct lc_monetary *monetary = locale->monetary;
#if WIDE
#define CONVERT_LOCALE_STRING(field, defval)    \
  const wchar_t *field = monetary->mon_##field; \
  if (field == NULL || *field == L'\0')         \
    field = defval;                             \
  size_t field##_len = wcslen(field)
#else
#define CONVERT_LOCALE_STRING(field, defval)                                  \
  char field[16];                                                             \
  size_t field##_len = 0;                                                     \
  do {                                                                        \
    const wchar_t *str = monetary->mon_##field;                               \
    if (str == NULL || *str == L'\0')                                         \
      str = defval;                                                           \
    const struct lc_ctype *ctype = locale->ctype;                             \
    while (*str != L'\0' && sizeof(field) - field##_len >= MB_LEN_MAX) {      \
      ssize_t len = ctype->c32tomb(&field[field##_len], *str++, ctype->data); \
      if (len < 0) {                                                          \
        field[field##_len++] = '?';                                           \
      } else {                                                                \
        field##_len += len;                                                   \
      }                                                                       \
    }                                                                         \
  } while (0)
#endif
  CONVERT_LOCALE_STRING(thousands_sep, L"");
  CONVERT_LOCALE_STRING(decimal_point, L".");
#undef CONVERT_LOCALE_STRING
  const wchar_t *positive_sign = monetary->positive_sign;
  if (positive_sign == NULL)
    positive_sign = L"";
  const wchar_t *negative_sign = monetary->negative_sign;
  if (negative_sign == NULL || *negative_sign == L'\0')
    negative_sign = L"-";

  // Process the format string.
  while (*format != '\0') {
    if (*format == '%') {
      ++format;

      // Parse flags.
      char_t fill_character = ' ';
      const signed char *mon_grouping = monetary->mon_grouping;
      bool use_parentheses = false, use_currency_symbol = true,
           left_justified = false;
      for (;;) {
        if (*format == '=') {
          fill_character = *++format;
        } else if (*format == '^') {
          mon_grouping = NULL;
        } else if (*format == '+') {
          use_parentheses = false;
        } else if (*format == '(') {
          use_parentheses = true;
        } else if (*format == '!') {
          use_currency_symbol = false;
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
      unsigned int right_precision = 0;
      bool have_right_precision = false;
      if (*format == '.') {
        ++format;
        right_precision = 0;
        while (*format >= '0' && *format <= '9')
          right_precision = right_precision * 10 + *format++ - '0';
        have_right_precision = true;
      }

      if (*format == 'i' || *format == 'n') {
        // Convert floating point value to decimal digits.
        double value = va_arg(ap, double);
        bool negative = signbit(value);
        bool international = *format == 'i';

        if (!have_right_precision) {
          // Use number of fractional digits from the locale if not
          // specified. If the locale does not offer a sane value, fall
          // back to using two.
          int frac_digits =
              international ? monetary->int_frac_digits : monetary->frac_digits;
          right_precision =
              frac_digits >= 0 && frac_digits != CHAR_MAX ? frac_digits : 2;
        }
        const wchar_t *currency_symbol;
        wchar_t int_curr_symbol[4];
        char separator = ' ';
        if (!use_currency_symbol) {
          // Don't use any currency symbol.
          currency_symbol = L"";
        } else if (international) {
          // Use the international currency symbol of the locale. The
          // first three characters correspond with the currency symbol.
          currency_symbol = int_curr_symbol;
          int_curr_symbol[0] = monetary->int_curr_symbol[0];
          int_curr_symbol[1] = monetary->int_curr_symbol[1];
          int_curr_symbol[2] = monetary->int_curr_symbol[2];
          int_curr_symbol[3] = L'\0';

          // The fourth is used as the separator character.
          separator = monetary->int_curr_symbol[3];
          if (separator == '\0')
            separator = ' ';
        } else {
          // Use the local currency symbol.
          currency_symbol = monetary->currency_symbol;
        }
        const wchar_t *sign = negative ? negative_sign : positive_sign;
        const wchar_t *opposite_sign = negative ? positive_sign : negative_sign;
#define LOCALE_ATTRIBUTE(name)                                           \
  char name =                                                            \
      international                                                      \
          ? (negative ? monetary->int_n_##name : monetary->int_p_##name) \
          : (negative ? monetary->n_##name : monetary->p_##name);        \
  char opposite_##name =                                                 \
      international                                                      \
          ? (negative ? monetary->int_p_##name : monetary->int_n_##name) \
          : (negative ? monetary->p_##name : monetary->n_##name)
        LOCALE_ATTRIBUTE(cs_precedes);
        LOCALE_ATTRIBUTE(sep_by_space);
        LOCALE_ATTRIBUTE(sign_posn);
#undef LOCALE_ATTRIBUTE
        if (use_parentheses) {
          // Force the use of parentheses.
          sign_posn = 0;
          opposite_sign_posn = 0;
        }

        // Generate the text that should appear before the value.
        char_t prefix[32];
        size_t prefixlen = generate_prefix(
            prefix, sizeof(prefix), currency_symbol, cs_precedes, sign,
            sign_posn, negative, separator, sep_by_space, locale);
        char_t opposite_prefix[32];
        size_t opposite_prefixlen = 0;
        if (left_precision > 0)
          opposite_prefixlen = generate_prefix(
              opposite_prefix, sizeof(opposite_prefix), currency_symbol,
              opposite_cs_precedes, opposite_sign, opposite_sign_posn,
              !negative, separator, opposite_sep_by_space, locale);

        // Generate the text that should appear after the value.
        char_t suffix[32];
        size_t suffixlen = generate_suffix(
            suffix, sizeof(suffix), currency_symbol, cs_precedes, sign,
            sign_posn, negative, separator, sep_by_space, locale);
        char_t opposite_suffix[32];
        size_t opposite_suffixlen = 0;
        if (left_precision > 0)
          opposite_suffixlen = generate_suffix(
              opposite_suffix, sizeof(opposite_suffix), currency_symbol,
              opposite_cs_precedes, opposite_sign, opposite_sign_posn,
              !negative, separator, opposite_sep_by_space, locale);

        // Convert floating point value to decimal digits.
        unsigned char digits[DECIMAL_DIG];
        size_t ndigits = sizeof(digits);
        int exponent;
        __f10dec(value, right_precision, digits, &ndigits, &exponent,
                 fegetround());

        // Determine the number of characters printed before the decimal point.
        struct numeric_grouping numeric_grouping;
        size_t left_digits_with_grouping = exponent >= 1 ? exponent : 1;
        left_digits_with_grouping +=
            numeric_grouping_init(&numeric_grouping, mon_grouping,
                                  left_digits_with_grouping) *
            thousands_sep_len;
        size_t left_precision_with_grouping =
            left_precision + numeric_grouping_init(&(struct numeric_grouping){},
                                                   mon_grouping,
                                                   left_precision) *
                                 thousands_sep_len;

        // Determine the total width of the value we are going to print.
        size_t width =
            (prefixlen > opposite_prefixlen ? prefixlen : opposite_prefixlen) +
            (left_digits_with_grouping > left_precision_with_grouping
                 ? left_digits_with_grouping
                 : left_precision_with_grouping) +
            (right_precision > 0 ? decimal_point_len + right_precision : 0) +
            (suffixlen > opposite_suffixlen ? suffixlen : opposite_suffixlen);

        // Print all of the padding, followed by the prefix.
        if (!left_justified) {
          while (field_width > width) {
            PUTCHAR(' ');
            --field_width;
          }
        }
        while (opposite_prefixlen-- > prefixlen)
          PUTCHAR(' ');
        for (size_t i = 0; i < prefixlen; ++i)
          PUTCHAR(prefix[i]);

        // Print fill characters at the start of the value.
        while (left_precision_with_grouping-- > left_digits_with_grouping)
          PUTCHAR(fill_character);

        // Print digits from the value.
        ssize_t position;
        ssize_t idx;
        if (exponent >= 1) {
          // At least one digit is placed before the radix character.
          position = -exponent;
          idx = 0;
        } else {
          // None of the digits are placed before the radix character.
          // Force zero padding.
          position = -1;
          idx = exponent - 1;
        }
        while (position < (ssize_t)right_precision) {
          unsigned char digit =
              idx >= 0 && (size_t)idx < ndigits ? digits[idx] : 0;
          if (position < 0) {
            // Print the grouping character.
            if (numeric_grouping_step(&numeric_grouping)) {
              for (size_t i = 0; i < thousands_sep_len; ++i)
                PUTCHAR(thousands_sep[i]);
            }
          } else if (position == 0) {
            // Print the radix character.
            for (size_t i = 0; i < decimal_point_len; ++i)
              PUTCHAR(decimal_point[i]);
          }
          PUTCHAR(digit + '0');
          ++position;
          ++idx;
        }
        assert(idx >= (ssize_t)ndigits && "Not all digits have been printed");

        // Print the suffix, followed by all the padding.
        for (size_t i = 0; i < suffixlen; ++i)
          PUTCHAR(suffix[i]);
        while (opposite_suffixlen-- > suffixlen)
          PUTCHAR(' ');
        while (field_width-- > width)
          PUTCHAR(' ');
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
#undef PUTCHAR
  return nwritten - 1;
}
