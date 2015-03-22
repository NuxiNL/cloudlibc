// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

const struct lc_ctype *ctype = locale->ctype;

while (*format != '\0') {
  if (*format == '%') {
    // Escaped percent symbol.
    if (*++format == '%') {
      PUTCHAR(*format);
      ++format;
      continue;
    }

    // Field number, in case of numbered arguments.
    PARSE_ARGNUM(arg_value);

    // Parse flags.
    char positive_sign = '\0';
    bool grouping = false, left_justified = false, alternative_form = false,
         zero_padding = false;
    for (;;) {
      if (*format == '\'') {
        grouping = true;
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
    unsigned char float_leading;
    unsigned char float_digits[F10DEC_DIGITS_MAX];
    size_t float_ndigits;
    int float_exponent;

    // Shared parameters for integer and floating point printing.
    char number_prefix[3] = {};  // "-", "0", "0x" or "-0x".
    size_t number_prefixlen = 0;
    const char *number_charset;

    // Parameters for string printing.
    char string_buf[2] = {};
    const char *string = NULL;

    // Parameters for wide string printing.
    wchar_t wstring_buf[2] = {};
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
          SET_NUMBER_PREFIX({'-'});
          integer_value = -value;
        }
        goto LABEL(integer);
      }
      case 'o': {
        // Octal integer.
        integer_base = 8;
        integer_value = GET_ARG_UINT_LM(length, arg_value);
        if (alternative_form && integer_value != 0)
          SET_NUMBER_PREFIX({'0'});
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
          SET_NUMBER_PREFIX({'0', 'x'});
        goto LABEL(integer);
      }
      case 'X': {
        // Hexadecimal integer, uppercase.
        integer_base = 16;
        integer_value = GET_ARG_UINT_LM(length, arg_value);
        if (alternative_form && integer_value != 0)
          SET_NUMBER_PREFIX({'0', 'X'});
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
              case 'a':
                // Hexadecimal floating point, lowercase.
                SET_NUMBER_PREFIX({negative ? '-' : positive_sign, '0', 'x'});
                float_exponent_char = 'p';
                goto LABEL(float16);
              case 'e':
                // Decimal floating point, exponential notation, lowercase.
                SET_NUMBER_PREFIX({negative ? '-' : positive_sign});
                float_exponent_char = 'e';
                goto LABEL(float10_exponential);
              default:
                // TODO(edje): Implement.
                PRINT_FIXED_STRING("unimplemented");
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
              case 'A':
                // Hexadecimal floating point, uppercase.
                SET_NUMBER_PREFIX({negative ? '-' : positive_sign, '0', 'X'});
                float_exponent_char = 'P';
                goto LABEL(float16);
              case 'E':
                // Decimal floating point, exponential notation, uppercase.
                SET_NUMBER_PREFIX({negative ? '-' : positive_sign});
                float_exponent_char = 'E';
                goto LABEL(float10_exponential);
              default:
                // TODO(edje): Implement.
                PRINT_FIXED_STRING("UNIMPLEMENTED");
            }
        }
      }
      case 'c': {
        // Character.
        if (length == LM_LONG) {
          wstring_buf[0] = GET_ARG_SINT_T(wchar_t, arg_value);
          wstring = wstring_buf;
          goto LABEL(wstring);
        } else {
          string_buf[0] = GET_ARG_SINT_T(int, arg_value);
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

        // Integer printing.
        LABEL(integer) : {
          // Convert integer to string representation. We generate up to
          // ~3 characters per byte. Take into account locale specific
          // grouping characters and 8 characters per byte should be a
          // safe upperbound.
          char digitsbuf[sizeof(uintmax_t) * 8];
          char *digits = digitsbuf + sizeof(digitsbuf);
          int grouping_chunk = integer_base == 10 && grouping ? 3 : 0;
          for (;;) {
            *--digits = number_charset[integer_value % integer_base];
            integer_value /= integer_base;
            if (integer_value == 0)
              break;

            // Use thousand separator.
            if (--grouping_chunk == 0) {
              // TODO(edje): Use thousand separator from LC_NUMERIC.
              // TODO(edje): Honour grouping from LC_NUMERIC.
              *--digits = ',';
              grouping_chunk = 3;
            }
          }

          // Determine width of the number, minus the padding.
          size_t width = digitsbuf + sizeof(digitsbuf) - digits;
          if ((ssize_t)width < precision)
            width = precision;
          width += number_prefixlen;

          // Print the prefix of the number, followed by zero padding if
          // a precision is specified, followed by the digits, followed
          // by padding if left-justified.
          if (!left_justified) {
            char padding = zero_padding && precision < 0 ? '0' : ' ';
            PAD_TO_FIELD_WIDTH(padding);
          }
          for (size_t i = 0; i < number_prefixlen; ++i)
            PUTCHAR(number_prefix[i]);
          while (precision-- > digitsbuf + sizeof(digitsbuf) - digits)
            PUTCHAR('0');
          while (digits < digitsbuf + sizeof(digitsbuf))
            PUTCHAR(*digits++);
          PAD_TO_FIELD_WIDTH(' ');
          break;
        }

        // Decimal floating point, using exponential notation.
        LABEL(float10_exponential) : {
          // Convert floating point number to digits.
          if (fpclassify(float_value) == FP_ZERO) {
            float_leading = 0;
            float_ndigits = 0;
            float_exponent = 0;
          } else {
            float_ndigits =
                precision >= 0 && precision < (ssize_t)sizeof(float_digits)
                    ? precision
                    : 6;
            __f10dec_exp(float_value, &float_leading, float_digits,
                         float_ndigits, &float_exponent);
          }
          goto LABEL(float_exponential);
        }

        // Hexadecimal floating point.
        LABEL(float16) : {
          // Convert floating point number to digits.
          if (fpclassify(float_value) == FP_ZERO) {
            float_leading = 0;
            float_ndigits = 0;
            float_exponent = 0;
          } else {
            float_leading = 1;
            float_ndigits =
                precision >= 0 && precision < (ssize_t)sizeof(float_digits)
                    ? precision
                    : sizeof(float_digits);
            f16dec(float_value, float_digits, &float_ndigits, &float_exponent,
                   fegetround());
          }
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
          for (;;) {
            *--exp_digits = number_charset[float_exponent % 10];
            float_exponent /= 10;
            if (float_exponent == 0)
              break;
          }

          // Determine width of the number as it would be printed, minus
          // the padding.
          size_t width =
              (ssize_t)float_ndigits > precision ? float_ndigits : precision;
          bool print_radixchar = alternative_form || width > 0;
          width += number_prefixlen + exp_digitsbuf + sizeof(exp_digitsbuf) -
                   exp_digits + (print_radixchar ? 4 : 3);

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
          PUTCHAR(float_leading + '0');
          // TODO(edje): Use radix character from LC_NUMERIC.
          if (print_radixchar)
            PUTCHAR('.');
          for (size_t i = 0; i < float_ndigits; ++i)
            PUTCHAR(number_charset[float_digits[i]]);
          while (precision-- > (ssize_t)float_ndigits)
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
            struct mbtoc32state ps = {};
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
            struct mbtoc32state ps = {};
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
  } else {
    PUTCHAR(*format);
    ++format;
  }
}
