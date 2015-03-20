// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

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
    char positive_sign = '\0', left_padding = ' ';
    bool grouping = false, left_justified = false, alternative_form = false;
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
        left_padding = '0';
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

    // State for number printing.
    char number_prefix[3] = {};  // "0", "0x" or "-0x".
    char number_midfix_buf[NUMBUF_SIZE];
    char *number_midfix = NULL;
    // char number_suffix[NUMBUF_SIZE]; // Exponent.

    // State for string printing.
    char string_buf[2] = {};
    const char *string = NULL;
    wchar_t wstring_buf[2] = {};
    const wchar_t *wstring = NULL;

    // Conversion specifiers.
    char_t specifier = *format++;
    switch (specifier) {
      case 'd':
      case 'i': {
        // Signed decimal integer.
        intmax_t value = GET_ARG_SINT_LM(length, arg_value);
        number_prefix[0] = value < 0 ? '-' : positive_sign;
        number_midfix = format_uint(value < 0 ? -value : value, 10, false,
                                    grouping, locale, number_midfix_buf);
        goto LABEL(number);
      }
      case 'o': {
        // Octal integer.
        uintmax_t value = GET_ARG_UINT_LM(length, arg_value);
        number_midfix =
            format_uint(value, 8, false, false, locale, number_midfix_buf);
        if (alternative_form && value != 0)
          number_prefix[0] = '0';
        goto LABEL(number);
      }
      case 'u': {
        // Unsigned decimal integer.
        uintmax_t value = GET_ARG_UINT_LM(length, arg_value);
        number_midfix =
            format_uint(value, 10, false, grouping, locale, number_midfix_buf);
        goto LABEL(number);
      }
      case 'x': {
        // Hexadecimal integer, lowercase.
        uintmax_t value = GET_ARG_UINT_LM(length, arg_value);
        if (alternative_form && value != 0) {
          number_prefix[0] = '0';
          number_prefix[1] = 'x';
        }
        number_midfix =
            format_uint(value, 16, false, false, locale, number_midfix_buf);
        goto LABEL(number);
      }
      case 'X': {
        // Hexadecimal integer, uppercase.
        uintmax_t value = GET_ARG_UINT_LM(length, arg_value);
        if (alternative_form && value != 0) {
          number_prefix[0] = '0';
          number_prefix[1] = 'X';
        }
        number_midfix =
            format_uint(value, 16, true, false, locale, number_midfix_buf);
        goto LABEL(number);
      }
      case 'f':
      case 'e':
      case 'g':
      case 'a': {
        // Floating point, lowercase.
        long double value = GET_ARG_FLOAT_LM(length, arg_value);
        bool negative = signbit(value);
        switch (fpclassify(value)) {
          case FP_INFINITE:
            string = negative ? "-inf" : "inf";
            goto LABEL(string);
          case FP_NAN:
            string = negative ? "-nan" : "nan";
            goto LABEL(string);
          default:
            // TODO(edje): Implement.
            string = "unimplemented";
            goto LABEL(string);
        }
      }
      case 'F':
      case 'E':
      case 'G':
      case 'A': {
        // Floating point, uppercase.
        long double value = GET_ARG_FLOAT_LM(length, arg_value);
        bool negative = signbit(value);
        switch (fpclassify(value)) {
          case FP_INFINITE:
            string = negative ? "-INF" : "INF";
            goto LABEL(string);
          case FP_NAN:
            string = negative ? "-NAN" : "NAN";
            goto LABEL(string);
          default:
            // TODO(edje): Implement.
            string = "unimplemented";
            goto LABEL(string);
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
        const void *value = GET_ARG_POINTER_T(void, arg_value);
        number_prefix[0] = '0';
        number_prefix[1] = 'x';
        precision = sizeof(void *) * 2;
        number_midfix = format_uint((uintptr_t)value, 16, false, false, locale,
                                    number_midfix_buf);
        goto LABEL(number);
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

        LABEL(number) : {
          // Number printing.
          size_t width = strnlen(number_prefix, sizeof(number_prefix));
          size_t buflen = number_midfix_buf + NUMBUF_SIZE - number_midfix;
          width += (ssize_t)buflen > precision ? buflen : (size_t)precision;
          // "If a precision is specified, the '0' flag shall be ignored."
          if (precision >= 0)
            left_padding = ' ';

          // If we have a padding character, make sure that we first print
          // the number prefix. We don't want a plus, minus or "0x" prefix
          // to be preceded by additional zeroes.
          if (left_padding != ' ') {
            for (size_t i = 0;
                 i < sizeof(number_prefix) && number_prefix[i] != '\0'; ++i)
              PUTCHAR(number_prefix[i]);
            number_prefix[0] = '\0';
          }

          // Add padding before value.
          if (!left_justified) {
            while (field_width > width) {
              PUTCHAR(left_padding);
              --field_width;
            }
          }

          // Print value.
          for (size_t i = 0;
               i < sizeof(number_prefix) && number_prefix[i] != '\0'; ++i)
            PUTCHAR(number_prefix[i]);
          while (precision-- > (ssize_t)buflen)
            PUTCHAR('0');
          while (number_midfix < number_midfix_buf + NUMBUF_SIZE) {
            PUTCHAR(*number_midfix);
            ++number_midfix;
          }

          // Add padding after value.
          while (field_width-- > width)
            PUTCHAR(' ');
          break;
        }

        LABEL(string) : {
          if (string == NULL)
            string = "(null)";
#if WIDE
          // TODO(edje): Implement.
          size_t width = 0;
#else
          size_t width = strnlen(string, precision);
#endif

          // Add padding before value.
          if (!left_justified) {
            while (field_width > width) {
              PUTCHAR(' ');
              --field_width;
            }
          }

#if WIDE
          // TODO(edje): Implement.
          errno = ENOSYS;
          goto bad;
#else
          for (size_t i = 0; i < width; ++i)
            PUTCHAR(string[i]);
#endif

          // Add padding after value.
          while (field_width-- > width)
            PUTCHAR(' ');
          break;
        }

        LABEL(wstring) : {
          if (wstring == NULL)
            wstring = L"(null)";
#if WIDE
          size_t width = wcsnlen(wstring, precision);
#else
          size_t width = 0;
          for (size_t i = 0;
               (precision < 0 || i < (size_t)precision) && wstring[i] != L'\0';
               ++i) {
            char buf[MB_LEN_MAX];
            const struct lc_ctype *ctype = locale->ctype;
            ssize_t len = ctype->c32tomb(buf, wstring[i], ctype->data);
            if (len < 0)
              goto bad;
            width += len;
          }
#endif

          // Add padding before value.
          if (!left_justified) {
            while (field_width > width) {
              PUTCHAR(' ');
              --field_width;
            }
          }

#if WIDE
          for (size_t i = 0; i < width; ++i)
            PUTCHAR(wstring[i]);
#else
          for (size_t i = 0;
               (precision < 0 || i < (size_t)precision) && wstring[i] != L'\0';
               ++i) {
            char buf[MB_LEN_MAX];
            const struct lc_ctype *ctype = locale->ctype;
            ssize_t len = ctype->c32tomb(buf, wstring[i], ctype->data);
            if (len < 0)
              goto bad;
            for (ssize_t j = 0; j < len; ++j)
              PUTCHAR(buf[j]);
          }
#endif

          // Add padding after value.
          while (field_width-- > width)
            PUTCHAR(' ');
          break;
        }
    }
  } else {
    PUTCHAR(*format);
    ++format;
  }
}
