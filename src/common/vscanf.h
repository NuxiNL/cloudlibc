// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/float10.h>
#include <common/float16.h>
#include <common/locale.h>
#include <common/mbstate.h>
#include <common/stdio.h>

#include <assert.h>
#include <fenv.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>

#if WIDE
typedef wchar_t char_t;
#else
typedef char char_t;
#endif

#include <common/vprintscanf.h>

#define VFSCANF 1
#define VSSCANF 2

#if STYLE == VSSCANF
// Updates the number of characters remaining in the input buffer to at
// least the number of characters requested.
static bool update_input_remaining(size_t *input_remaining, const char_t *s,
                                   size_t requested) {
  while (*input_remaining < requested && s[*input_remaining] != '\0') {
    ++*input_remaining;
  }
  return *input_remaining >= requested;
}
#endif

#define LM_SINT_MIN(length)        \
  ((length) == LM_SHORT_SHORT      \
       ? SCHAR_MIN                 \
       : (length) == LM_SHORT      \
             ? SHRT_MIN            \
             : (length) == LM_LONG \
                   ? LONG_MIN      \
                   : (length) == LM_LONG_LONG ? LLONG_MIN : INT_MIN)
#define LM_SINT_MAX(length)        \
  ((length) == LM_SHORT_SHORT      \
       ? SCHAR_MAX                 \
       : (length) == LM_SHORT      \
             ? SHRT_MAX            \
             : (length) == LM_LONG \
                   ? LONG_MAX      \
                   : (length) == LM_LONG_LONG ? LLONG_MAX : INT_MAX)
#define LM_UINT_MAX(length)        \
  ((length) == LM_SHORT_SHORT      \
       ? UCHAR_MAX                 \
       : (length) == LM_SHORT      \
             ? USHRT_MAX           \
             : (length) == LM_LONG \
                   ? ULONG_MAX     \
                   : (length) == LM_LONG_LONG ? ULLONG_MAX : UINT_MAX)

#if STYLE == VFSCANF
static int vfscanf_locked(FILE *stream, locale_t, const char_t *, va_list)
    __requires_exclusive(*stream);

int NAME(FILE *stream, locale_t locale, const char_t *format, va_list ap) {
  flockfile_orientation(stream, WIDE ? 1 : -1);
  int ret = vfscanf_locked(stream, locale, format, ap);
  funlockfile(stream);
  return ret;
}

static int vfscanf_locked(FILE *stream, locale_t locale, const char_t *format,
                          va_list ap) {
// TODO(ed): Make INPUT_REMAINING() work. Fix error handling.
#define INPUT_REMAINING(n) false
#define INPUT_PEEK(n) (stream->readbuf[n])
#define INPUT_SKIP(n)         \
  do {                        \
    fread_consume(stream, n); \
    input_read += (n);        \
  } while (0)
#elif STYLE == VSSCANF
int NAME(const char_t *restrict s, locale_t locale,
         const char_t *restrict format, va_list ap) {
  size_t input_remaining = 0;
#define INPUT_REMAINING(n) update_input_remaining(&input_remaining, s, n)
#define INPUT_PEEK(n) s[n]
#define INPUT_SKIP(n)                                                 \
  do {                                                                \
    assert((n) <= input_remaining && "Attempted to skip beyond end"); \
    s += (n);                                                         \
    input_remaining -= (n);                                           \
    input_read += (n);                                                \
  } while (0)
#else
#error "Unknown style"
#endif
  size_t input_read = 0;
  size_t conversions_performed = 0;

  // TODO(ed): Implement argument handling properly.
  void *arguments[10];
  for (size_t i = 0; i < 10; ++i)
    arguments[i] = va_arg(ap, void *);
  size_t argument_count = 0;
#define ARGUMENT_GET arguments[argument_count++]
#define ARGUMENT_SET_POINTER(value)   \
  do {                                \
    *(void **)ARGUMENT_GET = (value); \
  } while (0)
#define ARGUMENT_SET_INT(value)               \
  do {                                        \
    switch (length) {                         \
      case LM_SHORT_SHORT:                    \
        *(char *)ARGUMENT_GET = (value);      \
        break;                                \
      case LM_SHORT:                          \
        *(short *)ARGUMENT_GET = (value);     \
        break;                                \
      case LM_LONG:                           \
        *(long *)ARGUMENT_GET = (value);      \
        break;                                \
      case LM_LONG_LONG:                      \
        *(long long *)ARGUMENT_GET = (value); \
        break;                                \
      default:                                \
        *(int *)ARGUMENT_GET = (value);       \
        break;                                \
    }                                         \
  } while (0)

  while (*format != '\0') {
    if (*format == '%') {
      // Escaped percent symbol.
      if (*++format == '%') {
        // Skip leading whitespace, as required by the standard.
        size_t idx = 0;
#define SKIP(n) \
  do {          \
    idx += (n); \
  } while (0)
#define PEEK(n) (INPUT_REMAINING(idx + (n) + 1) ? INPUT_PEEK(idx + (n)) : '\0')
#include "parser_whitespace.h"
#undef PEEK
#undef SKIP

        // Match percent symbol.
        if (!INPUT_REMAINING(idx + 1) || INPUT_PEEK(idx) != *format++)
          return conversions_performed;
        INPUT_SKIP(idx + 1);
        continue;
      }

      // TODO(ed): Add support for numbered arguments.

      // Suppress assignment.
      bool suppress = false;
      if (*format == '*') {
        suppress = true;
        ++format;
      }

      // Maximum field width.
      size_t field_width = get_number(&format);

      // Allocate assignment.
      bool allocate = false;
      if (*format == 'm') {
        allocate = true;
        ++format;
      }

      // Length modifier.
      enum length_modifier length = get_length_modifier(&format);

      // Conversion specifiers.
      int base;
      switch (*format++) {
        case 'd':
          // Signed decimal integer.
          base = 10;
          goto signed_number;
        case 'i':
          // Signed integer.
          base = 0;
          goto signed_number;
        signed_number : {
          // Skip leading whitespace.
          size_t idx = 0;
#define SKIP(n) \
  do {          \
    idx += (n); \
  } while (0)
#define PEEK(n) (INPUT_REMAINING(idx + (n) + 1) ? INPUT_PEEK(idx + (n)) : '\0')
#include "parser_whitespace.h"
#undef PEEK

          // Determine up to where we can parse.
          size_t end = field_width == 0 ? SIZE_MAX : idx + field_width;
#define PEEK(n)                                                              \
  (idx + (n) < end && INPUT_REMAINING(idx + (n) + 1) ? INPUT_PEEK(idx + (n)) \
                                                     : '\0')
          typedef intmax_t int_t;
          intmax_t min = LM_SINT_MIN(length);
          intmax_t max = LM_SINT_MAX(length);
#include "parser_strtoint.h"
#undef PEEK
#undef SKIP

          if (!have_number)
            return conversions_performed;
          INPUT_SKIP(idx);
          ARGUMENT_SET_INT(number);
          ++conversions_performed;
          break;
        }

        case 'o':
          // Octal integer.
          base = 8;
          goto unsigned_number;
        case 'u':
          // Unsigned decimal integer.
          base = 10;
          goto unsigned_number;
        case 'x':
          // Hexadecimal integer.
          base = 16;
          goto unsigned_number;
        case 'p':
          // Pointer.
          base = 16;
          length = LM_INFERRED(uintptr_t);
          goto unsigned_number;
        unsigned_number : {
          // Skip leading whitespace.
          size_t idx = 0;
#define SKIP(n) \
  do {          \
    idx += (n); \
  } while (0)
#define PEEK(n) (INPUT_REMAINING(idx + (n) + 1) ? INPUT_PEEK(idx + (n)) : '\0')
#include "parser_whitespace.h"
#undef PEEK

          // Determine up to where we can parse.
          size_t end = field_width == 0 ? SIZE_MAX : idx + field_width;
#define PEEK(n)                                                              \
  (idx + (n) < end && INPUT_REMAINING(idx + (n) + 1) ? INPUT_PEEK(idx + (n)) \
                                                     : '\0')
          typedef uintmax_t int_t;
          uintmax_t min = 0;
          uintmax_t max = LM_UINT_MAX(length);
#include "parser_strtoint.h"
#undef PEEK
#undef SKIP

          if (!have_number)
            return conversions_performed;
          INPUT_SKIP(idx);
          ARGUMENT_SET_INT(number);
          ++conversions_performed;
          break;
        }

        case 'a':
        case 'e':
        case 'f':
        case 'g': {
          // Floating point.

          // Skip leading whitespace.
          size_t idx = 0;
#define SKIP(n) \
  do {          \
    idx += (n); \
  } while (0)
#define PEEK(n) (INPUT_REMAINING(idx + (n) + 1) ? INPUT_PEEK(idx + (n)) : '\0')
#include "parser_whitespace.h"
#undef PEEK

          // Determine up to where we can parse.
          size_t end = field_width == 0 ? SIZE_MAX : idx + field_width;
          typedef long double flt_t;
#define PEEK(n)                                                              \
  (idx + (n) < end && INPUT_REMAINING(idx + (n) + 1) ? INPUT_PEEK(idx + (n)) \
                                                     : '\0')
#include "parser_strtofloat.h"
#undef PEEK
#undef SKIP

          if (!have_number)
            return conversions_performed;
          INPUT_SKIP(idx);
          // TODO(ed): Store value.
          ++conversions_performed;
          break;
        }

        case 'S':
          // Wide string.
          length = LM_LONG;
        case 's': {
          // String.
          // TODO(ed): Implement.
          break;
        }

        case '[': {
          // Set of characters.
          // TODO(ed): Implement.
          break;
        }

        case 'C':
          // Fixed-size string of wide characters.
          length = LM_LONG;
        case 'c': {
          // Fixed-size string of characters.
          // TODO(ed): Add support for %lc.
          if (field_width == 0)
            field_width = 1;
          if (!INPUT_REMAINING(field_width))
            return conversions_performed;
          if (!suppress) {
            char_t *out;
            if (allocate) {
              // Allocate buffer for characters.
              out = malloc(field_width);
              if (out == NULL)
                return EOF;
              ARGUMENT_SET_POINTER(out);
            } else {
              // Store data directly.
              out = ARGUMENT_GET;
            }
            for (size_t i = 0; i < field_width; ++i)
              out[i] = INPUT_PEEK(i);
          }
          INPUT_SKIP(field_width);
          ++conversions_performed;
          break;
        }

        case 'n': {
          // Number of characters parsed.
          ARGUMENT_SET_INT(input_read);
          break;
        }
      }
    } else {
      // Perform exact match against character in format string.
      if (!INPUT_REMAINING(1) || INPUT_PEEK(0) != *format++)
        return conversions_performed;
      INPUT_SKIP(1);
    }
  }
  return conversions_performed;
}
