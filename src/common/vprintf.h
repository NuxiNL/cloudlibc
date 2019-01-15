// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/float10.h>
#include <common/float16.h>
#include <common/locale.h>
#include <common/mbstate.h>
#include <common/numeric_grouping.h>
#include <common/stdio.h>

#include <assert.h>
#include <errno.h>
#include <fenv.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#if WIDE
typedef wchar_t char_t;
#else
typedef char char_t;
#endif

#include <common/vprintscanf.h>

#define VASPRINTF 1
#define VDPRINTF 2
#define VFPRINTF 3
#define VSNPRINTF 4

#define GET_ARG_SINT_LM(lm, index)                                             \
  ((lm) == LM_SHORT_SHORT                                                      \
       ? (signed char)GET_ARG_SINT_T(int, index)                               \
       : (lm) == LM_SHORT                                                      \
             ? (signed short)GET_ARG_SINT_T(int, index)                        \
             : (lm) == LM_LONG ? GET_ARG_SINT_T(signed long, index)            \
                               : (lm) == LM_LONG_LONG                          \
                                     ? GET_ARG_SINT_T(signed long long, index) \
                                     : GET_ARG_SINT_T(signed int, index))
#define GET_ARG_UINT_LM(lm, index)                                   \
  ((lm) == LM_SHORT_SHORT                                            \
       ? (unsigned char)GET_ARG_UINT_T(int, index)                   \
       : (lm) == LM_SHORT                                            \
             ? (unsigned short)GET_ARG_UINT_T(int, index)            \
             : (lm) == LM_LONG                                       \
                   ? GET_ARG_UINT_T(unsigned long, index)            \
                   : (lm) == LM_LONG_LONG                            \
                         ? GET_ARG_UINT_T(unsigned long long, index) \
                         : GET_ARG_UINT_T(unsigned int, index))

#define GET_ARG_FLOAT_LM(lm, index)                             \
  ((lm) == LM_LONG_DOUBLE ? GET_ARG_FLOAT_T(long double, index) \
                          : GET_ARG_FLOAT_T(double, index))

// Scans through a format string and determines whether the format uses
// numbered arguments. If so, it returns the highest numbered argument used.
// This can be used to allocate space to store the numbered arguments.
static size_t get_numarg_max(const char_t *format) {
  size_t max_numarg = 0;
  while (*format != '\0') {
    if (*format++ == '%') {
      // Numbered argument value.
      bool got_numarg = false;
      {
        size_t numarg = get_numarg(&format);
        if (numarg > 0)
          got_numarg = true;
        if (max_numarg < numarg)
          max_numarg = numarg;
      }

      // Minimum field width and precision.
      while (strchr("diouxXfFeEgGaAcspCS%m", *format) == NULL) {
        if (*format++ == '*') {
          size_t numarg = get_numarg(&format);
          if (numarg == 0)
            return 0;
          if (max_numarg < numarg)
            max_numarg = numarg;
        }
      }

      // Non-numbered argument encountered.
      if (*format != '%' && *format != 'm' && !got_numarg)
        return 0;
    }
  }
  return max_numarg;
}

struct numarg_type {
  enum { K_SINT, K_UINT, K_POINTER, K_FLOAT } kind;
  enum length_modifier modifier;
};

// Scans through a format string and determines the types of the
// numbered arguments. These tyes can then be used by
// get_numarg_values() to extract the arguments of the call iteratively
// and storing them in an array, so they can be addressed randomly.
static void get_numarg_types(const char_t *format,
                             struct numarg_type *numarg_types) {
  while (*format != '\0') {
    if (*format++ == '%') {
      size_t arg_value = get_numarg(&format);

#if WIDE
      // Skip flags.
      format += wcsspn(format, L"'-+ #0");
#else
      format += strspn(format, "'-+ #0");
#endif

      // Minimum field width.
      if (*format == '*') {
        ++format;
        size_t arg_field_width = get_numarg(&format) - 1;
        numarg_types[arg_field_width].modifier = LM_DEFAULT;
        numarg_types[arg_field_width].kind = K_SINT;
      } else {
        get_number(&format);
      }

      // Precision.
      if (*format == '.') {
        ++format;
        if (*format == '*') {
          ++format;
          size_t arg_precision = get_numarg(&format) - 1;
          numarg_types[arg_precision].modifier = LM_DEFAULT;
          numarg_types[arg_precision].kind = K_SINT;
        } else {
          get_number(&format);
        }
      }

      // Not a numbered argument.
      char_t specifier = *format++;
      if (arg_value-- == 0)
        continue;

      // Length modifier.
      enum length_modifier length = get_length_modifier(&format);
      numarg_types[arg_value].modifier = length;

      // Conversion specifiers.
      switch (specifier) {
        case 'd':
        case 'i': {
          // Signed decimal integer.
          numarg_types[arg_value].kind = K_SINT;
          break;
        }
        case 'o':
        case 'u':
        case 'x':
        case 'X': {
          // Unsigned integer types.
          numarg_types[arg_value].kind = K_UINT;
          break;
        }
        case 'f':
        case 'F':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
        case 'a':
        case 'A': {
          // Floating point types.
          numarg_types[arg_value].kind = K_FLOAT;
          break;
        }
        case 'c': {
          // Character.
          if (length == LM_LONG) {
            numarg_types[arg_value].kind = K_SINT;
            numarg_types[arg_value].modifier = LM_WCHAR;
          } else {
            numarg_types[arg_value].kind = K_UINT;
            numarg_types[arg_value].modifier = LM_CHAR;
          }
          break;
        }
        case 'C': {
          // Wide character.
          numarg_types[arg_value].kind = K_SINT;
          numarg_types[arg_value].modifier = LM_WCHAR;
          break;
        }
        case 's':
        case 'p':
        case 'S': {
          // Pointers.
          numarg_types[arg_value].kind = K_POINTER;
          break;
        }
      }
    }
  }
}

union numarg_value {
  intmax_t v_sint;
  uintmax_t v_uint;
  const void *v_pointer;
  long double v_float;
};

// Extracts all the arguments in a va_list and stores them in an array
// that can be accessed randomly. It uses the typing information from
// get_numarg_types() to extract values elements with the right size and
// alignment from the va_list.
static void get_numarg_values(size_t numarg_max,
                              struct numarg_type *numarg_types,
                              union numarg_value *numarg_values, va_list ap) {
#define GET_ARG_SINT_T(type, index) va_arg(ap, type)
#define GET_ARG_UINT_T(type, index) va_arg(ap, type)
#define GET_ARG_POINTER_T(type, index) va_arg(ap, const type *)
#define GET_ARG_FLOAT_T(type, index) va_arg(ap, type)
  for (size_t i = 0; i < numarg_max; ++i) {
    switch (numarg_types[i].kind) {
      case K_SINT:
        numarg_values[i].v_sint = GET_ARG_SINT_LM(numarg_types[i].modifier, i);
        break;
      case K_UINT:
        numarg_values[i].v_uint = GET_ARG_UINT_LM(numarg_types[i].modifier, i);
        break;
      case K_POINTER:
        numarg_values[i].v_pointer = GET_ARG_POINTER_T(void, i);
        break;
      case K_FLOAT:
        numarg_values[i].v_float =
            GET_ARG_FLOAT_LM(numarg_types[i].modifier, i);
        break;
    }
  }
#undef GET_ARG_FLOAT_T
#undef GET_ARG_SINT_T
#undef GET_ARG_UINT_T
#undef GET_ARG_POINTER_T
}

#if STYLE == VASPRINTF
int NAME(char_t **s, locale_t locale, const char_t *format, va_list ap) {
  // Already preallocate a buffer of 16 bytes.
  size_t resultlen = 16 / sizeof(char_t);
  char_t *result = malloc(resultlen * sizeof(char_t));
  if (result == NULL)
    return -1;
  size_t resultstored = 0;
#define PUTCHAR(c)                                                     \
  do {                                                                 \
    if (resultstored == resultlen) {                                   \
      resultlen *= 2;                                                  \
      char_t *newresult = realloc(result, resultlen * sizeof(char_t)); \
      if (newresult == NULL) {                                         \
        free(result);                                                  \
        return -1;                                                     \
      }                                                                \
      result = newresult;                                              \
    }                                                                  \
    result[resultstored++] = (c);                                      \
  } while (0)
#elif STYLE == VDPRINTF
int NAME(int fd, locale_t locale, const char_t *format, va_list ap) {
  char result[BUFSIZ];
  size_t resultstored = 0;
  size_t resultwritten = 0;
#if WIDE
  static const mbstate_t initial_mbstate;
  mbstate_t resultmbs = initial_mbstate;
#define PUTCHAR(c)                                                      \
  do {                                                                  \
    size_t k = wcrtomb_l(result + resultstored, c, &resultmbs, locale); \
    if (k == (size_t)-1)                                                \
      return -1;                                                        \
    resultstored += k;                                                  \
    while (resultstored > sizeof(result) - MB_LEN_MAX) {                \
      ssize_t l = write(fd, result, resultstored);                      \
      if (l == -1)                                                      \
        return -1;                                                      \
      memmove(result, result + l, resultstored - l);                    \
      resultstored -= l;                                                \
    }                                                                   \
    ++resultwritten;                                                    \
  } while (0)
#else
#define PUTCHAR(c)                                   \
  do {                                               \
    result[resultstored++] = (c);                    \
    while (resultstored == sizeof(result)) {         \
      ssize_t l = write(fd, result, sizeof(result)); \
      if (l == -1)                                   \
        return -1;                                   \
      memmove(result, result + l, resultstored - l); \
      resultstored -= l;                             \
    }                                                \
    ++resultwritten;                                 \
  } while (0)
#endif
#elif STYLE == VFPRINTF
int NAME(FILE *stream, locale_t locale, const char_t *format, va_list ap) {
  size_t resultwritten = 0;
  flockfile_orientation(stream, WIDE ? 1 : -1);
#if WIDE
#define PUTCHAR(c)                           \
  do {                                       \
    if (putwc_unlocked(c, stream) == WEOF) { \
      funlockfile(stream);                   \
      return -1;                             \
    }                                        \
    ++resultwritten;                         \
  } while (0)
#else
#define PUTCHAR(c)                         \
  do {                                     \
    if (putc_unlocked(c, stream) == EOF) { \
      funlockfile(stream);                 \
      return -1;                           \
    }                                      \
    ++resultwritten;                       \
  } while (0)
#endif
#elif STYLE == VSNPRINTF
int NAME(char_t *s, size_t n, locale_t locale, const char_t *format,
         va_list ap) {
  size_t resultstored = 0;
#define PUTCHAR(c)            \
  do {                        \
    char_t ch = (c);          \
    if (resultstored + 1 < n) \
      s[resultstored] = ch;   \
    ++resultstored;           \
  } while (0)
#else
#error "Unknown style"
#endif
  // Save current errno for %m.
  int saved_errno = errno;

  size_t numarg_max = get_numarg_max(format);
  if (numarg_max > 0) {
    // Numbered arguments require us to construct a table with all
    // argument values explicitly, as va_lists cannot be accessed
    // randomly. Furthermore, we cannot iterate over the va_list without
    // knowing the exact type of the parameters.
    union numarg_value numarg_values[numarg_max];
    {
      // Start out by inferring the type of all the parameters.
      struct numarg_type numarg_types[numarg_max];
      get_numarg_types(format, numarg_types);
      // Now iterate over the va_list to extract the values using the
      // proper type.
      get_numarg_values(numarg_max, numarg_types, numarg_values, ap);
    }
#define PARSE_ARGNUM(field) size_t field = get_numarg(&format) - 1
#define GET_ARG_SINT_T(type, index) ((type)numarg_values[index].v_sint)
#define GET_ARG_UINT_T(type, index) ((type)numarg_values[index].v_uint)
#define GET_ARG_POINTER_T(type, index) \
  ((const type *)numarg_values[index].v_pointer)
#define GET_ARG_FLOAT_T(type, index) ((type)numarg_values[index].v_float)
#define LABEL(n) n##_1
#include "vprintf_body.h"
#undef PARSE_ARGNUM
#undef GET_ARG_SINT_T
#undef GET_ARG_UINT_T
#undef GET_ARG_POINTER_T
#undef GET_ARG_FLOAT_T
#undef LABEL
  } else {
#define PARSE_ARGNUM(field)
#define GET_ARG_SINT_T(type, index) va_arg(ap, type)
#define GET_ARG_UINT_T(type, index) va_arg(ap, type)
#define GET_ARG_POINTER_T(type, index) va_arg(ap, const type *)
#define GET_ARG_FLOAT_T(type, index) va_arg(ap, type)
#define LABEL(n) n##_2
#include "vprintf_body.h"
#undef PARSE_ARGNUM
#undef GET_ARG_SINT_T
#undef GET_ARG_UINT_T
#undef GET_ARG_POINTER_T
#undef GET_ARG_FLOAT_T
#undef LABEL
  }
#if STYLE == VASPRINTF
  // Nul-terminate the buffer.
  PUTCHAR('\0');
  *s = result;
  return resultstored - 1;
bad:
  free(result);
  return -1;
#elif STYLE == VDPRINTF
  while (resultstored > 0) {
    ssize_t l = write(fd, result, resultstored);
    if (l == -1)
      return -1;
    memmove(result, result + l, resultstored - l);
    resultstored -= l;
  }
  return resultwritten;
bad:
  return -1;
#elif STYLE == VFPRINTF
  funlockfile(stream);
  return resultwritten;
bad:
  funlockfile(stream);
  return -1;
#elif STYLE == VSNPRINTF
  // Nul-terminate the buffer, if provided.
  if (n > 0)
    s[resultstored < n - 1 ? resultstored : n - 1] = '\0';
  return resultstored;
bad:
  return -1;
#endif
}
