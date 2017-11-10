// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

const struct lc_ctype *ctype = locale->ctype;
const struct lc_messages *messages = locale->messages;
const struct lc_numeric *numeric = locale->numeric;

while (*format != '\0') {
  if (*format == '%') {
    ++format;
    // Field number, in case of numbered arguments.
    PARSE_ARGNUM(arg_value);

    // Parse flags.
    const signed char *grouping = NULL;
    char positive_sign = '\0';
    bool left_justified = false, alternative_form = false, zero_padding = false;
    for (;;) {
      if (*format == '\'') {
        grouping = numeric->grouping;
      } else if (*format == '-') {
        left_justified = true;
      } else if (*format == '+') {
        positive_sign = '+';
      } else if (*format == ' ') {
        if (positive_sign != '+')
          positive_sign = ' ';
      } else if (*format == '#') {
        alternative_form = true;
      } else if (*format == '0') {
        zero_padding = true;
      } else {
        break;
      }
      ++format;
    }
    if (left_justified)
      zero_padding = false;

    // Minimum field width.
    size_t field_width;
    if (*format == '*') {
      ++format;
      PARSE_ARGNUM(arg_field_width);
      field_width = GET_ARG_SINT_T(int, arg_field_width);
    } else {
      field_width = get_number(&format);
    }

    // Precision.
    int precision = -1;
    if (*format == '.') {
      ++format;
      if (*format == '*') {
        ++format;
        PARSE_ARGNUM(arg_precision);
        precision = GET_ARG_SINT_T(int, arg_precision);
      } else {
        precision = get_number(&format);
      }
    }

    // Length modifier.
    enum length_modifier length = get_length_modifier(&format);

    // Parameters for integer printing.
    uintmax_t integer_value;
    unsigned int integer_base;

    // Parameters for floating point printing.
    long double float_value;
    char float_exponent_char;
    unsigned char float_digits[DECIMAL_DIG];
    size_t float_ndigits;
    int float_exponent;
    int float_exponent_mindigits;
    bool float_strip_trailing = false;

    // Shared parameters for integer and floating point printing.
    char number_prefix[3] = {};  // "-", "0", "0x" or "-0x".
    size_t number_prefixlen = 0;
    const char *number_charset;

    // Parameters for string printing.
    char string_buf[NL_TEXTMAX];
    const char *string = NULL;

    // Parameters for wide string printing.
    wchar_t wstring_buf[2];
    const wchar_t *wstring = NULL;

#define SET_NUMBER_PREFIX(...)                      \
  do {                                              \
    const char str[] = __VA_ARGS__;                 \
    number_prefixlen = 0;                           \
    for (size_t i = 0; i < sizeof(str); ++i)        \
      if (str[i] != '\0')                           \
        number_prefix[number_prefixlen++] = str[i]; \
  } while (0)
#define PAD_TO_FIELD_WIDTH(padding) \
  do {                              \
    while (field_width > width) {   \
      PUTCHAR(padding);             \
      --field_width;                \
    }                               \
  } while (0)
#if WIDE
#define PRINT_FIXED_STRING(str) \
  do {                          \
    wstring = L##str;           \
    goto LABEL(wstring);        \
  } while (0)
#else
#define PRINT_FIXED_STRING(str) \
  do {                          \
    string = str;               \
    goto LABEL(string);         \
  } while (0)
#endif

    // Parse conversion specifier.
    char_t specifier = *format++;
    number_charset = specifier >= 'a' ? "0123456789abcdef" : "0123456789ABCDEF";
    switch (specifier) {
      case 'd':
      case 'i': {
        // Signed decimal integer.
        integer_base = 10;
        intmax_t value = GET_ARG_SINT_LM(length, arg_value);
        if (value >= 0) {
          SET_NUMBER_PREFIX({positive_sign});
          integer_value = value;
        } else {
          SET_NUMBER_PREFIX("-");
          integer_value = -value;
        }
        goto LABEL(integer);
      }
      case 'o': {
        // Octal integer.
        integer_base = 8;
        integer_value = GET_ARG_UINT_LM(length, arg_value);
        if (alternative_form && integer_value != 0)
          SET_NUMBER_PREFIX("0");
        goto LABEL(integer);
      }
      case 'u': {
        // Unsigned decimal integer.
        integer_base = 10;
        integer_value = GET_ARG_UINT_LM(length, arg_value);
        goto LABEL(integer);
      }
      case 'x': {
        // Hexadecimal integer, lowercase.
        integer_base = 16;
        integer_value = GET_ARG_UINT_LM(length, arg_value);
        if (alternative_form && integer_value != 0)
          SET_NUMBER_PREFIX("0x");
        goto LABEL(integer);
      }
      case 'X': {
        // Hexadecimal integer, uppercase.
        integer_base = 16;
        integer_value = GET_ARG_UINT_LM(length, arg_value);
        if (alternative_form && integer_value != 0)
          SET_NUMBER_PREFIX("0X");
        goto LABEL(integer);
      }
      case 'f':
      case 'e':
      case 'g':
      case 'a': {
        // Floating point, lowercase.
        float_value = GET_ARG_FLOAT_LM(length, arg_value);
        bool negative = signbit(float_value);
        switch (fpclassify(float_value)) {
          case FP_INFINITE:
            if (negative)
              PRINT_FIXED_STRING("-inf");
            else
              PRINT_FIXED_STRING("inf");
          case FP_NAN:
            if (negative)
              PRINT_FIXED_STRING("-nan");
            else
              PRINT_FIXED_STRING("nan");
          default:
            switch (specifier) {
              case 'f':
                // Decimal floating point, without exponent.
                goto LABEL(float10);
              case 'e':
                // Decimal floating point, exponential notation, lowercase.
                SET_NUMBER_PREFIX({negative ? '-' : positive_sign});
                float_exponent_char = 'e';
                goto LABEL(float10_exponential);
              case 'g':
                // Decimal floating point, with or without exponent, lowercase.
                SET_NUMBER_PREFIX({negative ? '-' : positive_sign});
                float_exponent_char = 'e';
                goto LABEL(float10_auto);
              case 'a':
                // Hexadecimal floating point, lowercase.
                SET_NUMBER_PREFIX({negative ? '-' : positive_sign, '0', 'x'});
                float_exponent_char = 'p';
                goto LABEL(float16);
            }
        }
      }
      case 'F':
      case 'E':
      case 'G':
      case 'A': {
        // Floating point, uppercase.
        float_value = GET_ARG_FLOAT_LM(length, arg_value);
        bool negative = signbit(float_value);
        switch (fpclassify(float_value)) {
          case FP_INFINITE:
            if (negative)
              PRINT_FIXED_STRING("-INF");
            else
              PRINT_FIXED_STRING("INF");
          case FP_NAN:
            if (negative)
              PRINT_FIXED_STRING("-NAN");
            else
              PRINT_FIXED_STRING("NAN");
          default:
            switch (specifier) {
              case 'F':
                // Decimal floating point, without exponent.
                goto LABEL(float10);
              case 'E':
                // Decimal floating point, exponential notation, uppercase.
                SET_NUMBER_PREFIX({negative ? '-' : positive_sign});
                float_exponent_char = 'E';
                goto LABEL(float10_exponential);
              case 'G':
                // Decimal floating point, with or without exponent, uppercase.
                SET_NUMBER_PREFIX({negative ? '-' : positive_sign});
                float_exponent_char = 'E';
                goto LABEL(float10_auto);
              case 'A':
                // Hexadecimal floating point, uppercase.
                SET_NUMBER_PREFIX({negative ? '-' : positive_sign, '0', 'X'});
                float_exponent_char = 'P';
                goto LABEL(float16);
            }
        }
      }
      case 'c': {
        // Character.
        if (length == LM_LONG) {
          wstring_buf[0] = GET_ARG_SINT_T(wchar_t, arg_value);
          string_buf[1] = L'\0';
          wstring = wstring_buf;
          goto LABEL(wstring);
        } else {
          string_buf[0] = GET_ARG_SINT_T(int, arg_value);
          string_buf[1] = '\0';
          string = string_buf;
          goto LABEL(string);
        }
      }
      case 's': {
        // String.
        if (length == LM_LONG) {
          wstring = GET_ARG_POINTER_T(wchar_t, arg_value);
          goto LABEL(wstring);
        } else {
          string = GET_ARG_POINTER_T(char, arg_value);
          goto LABEL(string);
        }
      }
      case 'p': {
        // Pointer.
        integer_base = 16;
        integer_value = (uintptr_t)GET_ARG_POINTER_T(void, arg_value);
        SET_NUMBER_PREFIX({'0', 'x'});
        goto LABEL(integer);
      }
      case 'C': {
        // Wide character.
        wstring_buf[0] = GET_ARG_SINT_T(wchar_t, arg_value);
        wstring = wstring_buf;
        goto LABEL(wstring);
      }
      case 'S': {
        // Wide string.
        wstring = GET_ARG_POINTER_T(wchar_t, arg_value);
        goto LABEL(wstring);
      }
      case 'm': {
        // Extension: error message strings, used by syslog().
        if (saved_errno >= 0 &&
            saved_errno < (int)__arraycount(messages->strerror) &&
            messages->strerror[saved_errno] != NULL) {
          __locale_translate_string(locale, string_buf,
                                    messages->strerror[saved_errno],
                                    sizeof(string_buf));
        } else {
          __locale_translate_string(locale, string_buf, messages->unknown_error,
                                    sizeof(string_buf));
        }
        string = string_buf;
        goto LABEL(string);
      }
      case '%': {
        // Percent symbol.
        PUTCHAR('%');
        break;
      }

        // Integer printing.
        LABEL(integer) : {
          // Convert integer to string representation. We generate up to
          // 3 characters per byte, as the base is at least 8.
          char digitsbuf[sizeof(uintmax_t) * 3];
          char *digits = digitsbuf + sizeof(digitsbuf);
          for (;;) {
            *--digits = number_charset[integer_value % integer_base];
            integer_value /= integer_base;
            if (integer_value == 0)
              break;
          }

          // Determine width of the number, minus the padding. Take into
          // account the number of grouping characters we need to insert
          // into the number.
          size_t width = digitsbuf + sizeof(digitsbuf) - digits;
          struct numeric_grouping numeric_grouping;
          width += numeric_grouping_init(&numeric_grouping, grouping, width) *
                   1;  // TODO(ed): Use the proper width.
          if ((ssize_t)width < precision)
            width = precision;
          width += number_prefixlen;

          // Print the prefix of the number, followed by zero padding if
          // a precision is specified, followed by the digits, followed
          // by padding if left-justified.
          if (zero_padding && precision < 0) {
            for (size_t i = 0; i < number_prefixlen; ++i)
              PUTCHAR(number_prefix[i]);
            PAD_TO_FIELD_WIDTH('0');
          } else {
            if (!left_justified)
              PAD_TO_FIELD_WIDTH(' ');
            for (size_t i = 0; i < number_prefixlen; ++i)
              PUTCHAR(number_prefix[i]);
          }
          while (precision-- > digitsbuf + sizeof(digitsbuf) - digits)
            PUTCHAR('0');
          while (digits < digitsbuf + sizeof(digitsbuf)) {
            if (numeric_grouping_step(&numeric_grouping)) {
              // Add thousands separator.
              // TODO(ed): Deal with multibyte!
              PUTCHAR(numeric->thousands_sep[0]);
            }
            PUTCHAR(*digits++);
          }
          PAD_TO_FIELD_WIDTH(' ');
          break;
        }

        // Decimal floating point, without exponent.
        LABEL(float10) : {
          if (precision < 0)
            precision = 6;
          float_ndigits = sizeof(float_digits);
          SET_NUMBER_PREFIX({signbit(float_value) ? '-' : positive_sign});
          __f10dec(float_value, precision, float_digits, &float_ndigits,
                   &float_exponent, fegetround());

          // %g without #: strip trailing zeroes. Implement this by
          // decreasing the precision to the last non-zero decimal, or
          // zero if there are none.
          if (float_strip_trailing) {
            if (float_exponent > (int)float_ndigits)
              precision = 0;
            else if (precision > (int)float_ndigits - float_exponent)
              precision = float_ndigits - float_exponent;
          }
          bool print_radixchar = alternative_form || precision > 0;

          // Determine the number of characters printed before the decimal
          // point.
          struct numeric_grouping numeric_grouping;
          size_t left_digits_with_grouping =
              float_exponent >= 1 ? float_exponent : 1;
          left_digits_with_grouping +=
              numeric_grouping_init(&numeric_grouping, grouping,
                                    left_digits_with_grouping) *
              1;  // TODO(ed): Use the proper width.
          size_t width = number_prefixlen + left_digits_with_grouping +
                         (print_radixchar ? 1 : 0) + precision;

          // Print the number.
          if (zero_padding) {
            for (size_t i = 0; i < number_prefixlen; ++i)
              PUTCHAR(number_prefix[i]);
            PAD_TO_FIELD_WIDTH('0');
          } else {
            if (!left_justified)
              PAD_TO_FIELD_WIDTH(' ');
            for (size_t i = 0; i < number_prefixlen; ++i)
              PUTCHAR(number_prefix[i]);
          }
          ssize_t position;
          ssize_t idx;
          if (float_exponent >= 1) {
            // At least one digit is placed before the radix character.
            position = -float_exponent;
            idx = 0;
          } else {
            // None of the digits are placed before the radix character.
            // Force zero padding.
            position = -1;
            idx = float_exponent - 1;
          }
          while (position < precision) {
            if (position < 0) {
              // Print the grouping character.
              if (numeric_grouping_step(&numeric_grouping)) {
                // TODO(ed): Deal with multibyte!
                PUTCHAR(numeric->thousands_sep[0]);
              }
            }
            unsigned char digit =
                idx >= 0 && (size_t)idx < float_ndigits ? float_digits[idx] : 0;
            PUTCHAR(digit + '0');
            ++idx;
            if (++position == 0 && print_radixchar) {
              // Print the radix character.
              // TODO(ed): Deal with multibyte!
              PUTCHAR(numeric->decimal_point[0]);
            }
          }
          assert(idx >= (ssize_t)float_ndigits &&
                 "Not all digits have been printed");
          PAD_TO_FIELD_WIDTH(' ');
          break;
        }

        // Decimal floating point, using exponential notation.
        LABEL(float10_exponential) : {
          // Convert floating point value to a sequence of decimal digits.
          if (precision < 0)
            precision = 6;
          float_ndigits = precision < (int)sizeof(float_digits)
                              ? precision + 1
                              : sizeof(float_digits);
          __f10dec(float_value, UINT_MAX, float_digits, &float_ndigits,
                   &float_exponent, fegetround());
          --float_exponent;
          float_exponent_mindigits = 2;
          goto LABEL(float_exponential);
        }

        // Decimal floating point, with or without exponent.
        LABEL(float10_auto) : {
          // See what the exponent would be if converted with %e.
          if (precision < 0)
            precision = 6;
          else if (precision == 0)
            precision = 1;
          --precision;
          float_ndigits = precision < (int)sizeof(float_digits)
                              ? precision + 1
                              : sizeof(float_digits);
          __f10dec(float_value, UINT_MAX, float_digits, &float_ndigits,
                   &float_exponent, fegetround());
          --float_exponent;
          if (precision >= float_exponent && float_exponent >= -4) {
            // Switch over to %f.
            precision -= float_exponent;
            if (!alternative_form)
              float_strip_trailing = true;
            goto LABEL(float10);
          }
          // Continue with %e.
          float_exponent_mindigits = 2;
          goto LABEL(float_exponential);
        }

        // Hexadecimal floating point.
        LABEL(float16) : {
          if (fpclassify(float_value) == FP_ZERO) {
            // Just zero digits.
            float_ndigits = 0;
            float_exponent = 0;
          } else {
            // No digits available.
            float_digits[0] = 1;
            float_ndigits =
                precision >= 0 && precision < (int)sizeof(float_digits)
                    ? precision
                    : sizeof(float_digits);
            f16dec(float_value, float_digits + 1, &float_ndigits,
                   &float_exponent, fegetround());
            ++float_ndigits;
          }
          float_exponent_mindigits = 1;
          goto LABEL(float_exponential);
        }

        // Exponentially formatted floating point numbers.
        LABEL(float_exponential) : {
          // Convert exponent to digits.
          bool exp_negative = false;
          if (float_exponent < 0) {
            exp_negative = true;
            float_exponent = -float_exponent;
          }
          char exp_digitsbuf[sizeof(int) * 3];
          char *exp_digits = exp_digitsbuf + sizeof(exp_digitsbuf);
          while (float_exponent_mindigits-- > 0 || float_exponent != 0) {
            *--exp_digits = number_charset[float_exponent % 10];
            float_exponent /= 10;
          }

          // Always make sure to print at least a single digit.
          if (float_ndigits == 0) {
            float_digits[0] = 0;
            float_ndigits = 1;
          }

          // Determine width of the number as it would be printed, minus
          // the padding.
          size_t width = precision + 1 > (ssize_t)float_ndigits ? precision + 1
                                                                : float_ndigits;
          bool print_radixchar = alternative_form || width > 1;
          width += number_prefixlen + exp_digitsbuf + sizeof(exp_digitsbuf) -
                   exp_digits + (print_radixchar ? 3 : 2);

          // Print the number.
          if (zero_padding) {
            for (size_t i = 0; i < number_prefixlen; ++i)
              PUTCHAR(number_prefix[i]);
            PAD_TO_FIELD_WIDTH('0');
          } else {
            if (!left_justified)
              PAD_TO_FIELD_WIDTH(' ');
            for (size_t i = 0; i < number_prefixlen; ++i)
              PUTCHAR(number_prefix[i]);
          }
          PUTCHAR(number_charset[float_digits[0]]);
          // TODO(ed): Deal with multibyte!
          if (print_radixchar)
            PUTCHAR(numeric->decimal_point[0]);
          for (size_t i = 1; i < float_ndigits; ++i)
            PUTCHAR(number_charset[float_digits[i]]);
          while (precision-- >= (ssize_t)float_ndigits)
            PUTCHAR('0');
          PUTCHAR(float_exponent_char);
          PUTCHAR(exp_negative ? '-' : '+');
          while (exp_digits < exp_digitsbuf + sizeof(exp_digitsbuf))
            PUTCHAR(*exp_digits++);
          PAD_TO_FIELD_WIDTH(' ');
          break;
        }

        // String printing.
        LABEL(string) : {
          // Extension: print "(null)" instead of dereferencing a null
          // pointer.
          if (string == NULL)
            string = "(null)";

          if (left_justified) {
            // String is left-justified. Print characters from the
            // string until the precision is reached.
            size_t width = 0;
#if WIDE
            mbstate_t ps;
            mbstate_set_init(&ps);
            while (width < (size_t)precision) {
              char32_t c32;
              ssize_t len =
                  ctype->mbtoc32(&c32, string, SIZE_MAX, &ps, ctype->data);
              if (len < 0)
                goto bad;
              if (c32 == U'\0')
                break;
              PUTCHAR(c32);
              string += len;
              ++width;
            }
#else
            while (width < (size_t)precision && *string != L'\0') {
              PUTCHAR(*string++);
              ++width;
            }
#endif
            PAD_TO_FIELD_WIDTH(' ');
          } else {
#if WIDE
            // String is right-justified. First compute the length to
            // determine how much padding we can write on the left.
            size_t width = 0;
            mbstate_t ps;
            mbstate_set_init(&ps);
            const char *string_end = string;
            while (width < (size_t)precision) {
              char32_t c32;
              ssize_t len =
                  ctype->mbtoc32(&c32, string_end, SIZE_MAX, &ps, ctype->data);
              if (len < 0)
                goto bad;
              if (c32 == U'\0')
                break;
              string_end += len;
              ++width;
            }
#else
            size_t width = strnlen(string, precision);
#endif
            PAD_TO_FIELD_WIDTH(' ');
#if WIDE
            // Print the string after the padding.
            mbstate_set_init(&ps);
            while (string < string_end) {
              char32_t c32;
              ssize_t len =
                  ctype->mbtoc32(&c32, string, SIZE_MAX, &ps, ctype->data);
              if (len < 0)
                goto bad;
              if (c32 == U'\0')
                break;
              PUTCHAR(c32);
              string += len;
            }
#else
            for (size_t i = 0; i < width; ++i)
              PUTCHAR(string[i]);
#endif
          }
          break;
        }

        // Wide string printing.
        LABEL(wstring) : {
          // Extension: print "(null)" instead of dereferencing a null
          // pointer.
          if (wstring == NULL)
            wstring = L"(null)";

          if (left_justified) {
            // String is left-justified. Print characters from the
            // string until the precision is reached.
            size_t width = 0;
            while (width < (size_t)precision && *wstring != L'\0') {
#if WIDE
              PUTCHAR(*wstring++);
              ++width;
#else
              char buf[MB_LEN_MAX];
              ssize_t len = ctype->c32tomb(buf, *wstring++, ctype->data);
              if (len < 0)
                goto bad;
              if (width + len > (size_t)precision)
                break;
              for (ssize_t j = 0; j < len; ++j)
                PUTCHAR(buf[j]);
              width += len;
#endif
            }
            PAD_TO_FIELD_WIDTH(' ');
          } else {
#if WIDE
            // String is right-justified. First compute the length to
            // determine how much padding we can write on the left.
            size_t width = wcsnlen(wstring, precision);
#else
            size_t width = 0;
            const wchar_t *wstring_end = wstring;
            while (width < (size_t)precision && *wstring_end != L'\0') {
              char buf[MB_LEN_MAX];
              ssize_t len = ctype->c32tomb(buf, *wstring_end++, ctype->data);
              if (len < 0)
                goto bad;
              if (width + len > (size_t)precision)
                break;
              width += len;
            }
#endif
            PAD_TO_FIELD_WIDTH(' ');
#if WIDE
            // Print the string after the padding.
            for (size_t i = 0; i < width; ++i)
              PUTCHAR(wstring[i]);
#else
            while (wstring < wstring_end) {
              char buf[MB_LEN_MAX];
              ssize_t len = ctype->c32tomb(buf, *wstring++, ctype->data);
              for (ssize_t j = 0; j < len; ++j)
                PUTCHAR(buf[j]);
            }
#endif
          }
          break;
        }
    }
#undef SET_NUMBER_PREFIX
#undef PAD_TO_FIELD_WIDTH
#undef PRINT_FIXED_STRING
  } else {
    PUTCHAR(*format);
    ++format;
  }
}
