// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/types.h>

#include <errno.h>
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
        // TODO(edje): Implement.
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
