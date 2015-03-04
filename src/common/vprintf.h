// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/float.h>
#include <common/locale.h>
#include <common/stdio.h>

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <locale.h>
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

// TODO(edje): Pick right value.
#define NUMBUF_SIZE 128

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

// Parses the "n$" part of a numerical argument reference of a format
// string. Upon success, this function returns a number greater than
// zero indicating the index of the numerical argument. Upon failure it
// returns zero.
static size_t get_numarg(const char_t **format) {
  const char_t *s = *format;
  if (*s < '1' || *s > '9')
    return 0;
  size_t value = 0;
  do {
    value = value * 10 + *s++ - '0';
  } while (*s >= '0' && *s <= '9');
  if (*s++ != '$')
    return 0;
  *format = s;
  return value;
}

// Scans through a format string and determines whether the format uses
// numbered arguments. If so, it returns the highest numbered argument used.
// This can be used to allocate space to store the numbered arguments.
static size_t get_numarg_max(const char_t *format) {
#ifdef NO_NUMARG
  return 0;
#else
  size_t max_numarg = 0;
  while (*format != '\0') {
    if (*format++ == '%') {
      if (*format == '%') {
        ++format;
      } else {
        // Value.
        {
          size_t numarg = get_numarg(&format);
          if (numarg == 0)
            return 0;
          if (max_numarg < numarg)
            max_numarg = numarg;
        }

        // Minimum field width and precision.
        while (strchr("diouxXfFeEgGaAcspCS%", *format) == NULL) {
          if (*format++ == '*') {
            size_t numarg = get_numarg(&format);
            if (numarg == 0)
              return 0;
            if (max_numarg < numarg)
              max_numarg = numarg;
          }
        }
      }
    }
  }
  return max_numarg;
#endif
}

struct numarg_type {
  enum { K_SINT, K_UINT, K_POINTER, K_FLOAT } kind;
  enum length_modifier modifier;
};

static void get_numarg_types(const char_t *format,
                             struct numarg_type *numarg_types) {
  while (*format != '\0') {
    if (*format++ == '%') {
      if (*format == '%') {
        ++format;
      } else {
        size_t arg_value = get_numarg(&format) - 1;

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

        // Length modifier.
        enum length_modifier length = get_length_modifier(&format);
        numarg_types[arg_value].modifier = length;

        // Conversion specifiers.
        switch (*format++) {
          case 'd':
          case 'i': {
            // Signed decimal integer.
            numarg_types[arg_value].kind = K_SINT;
            break;
          }
          case 'o': {
            // Octal integer.
            numarg_types[arg_value].kind = K_UINT;
            break;
          }
          case 'u': {
            // Unsigned decimal integer.
            numarg_types[arg_value].kind = K_UINT;
            break;
          }
          case 'x': {
            // Hexadecimal integer, lowercase.
            numarg_types[arg_value].kind = K_UINT;
            break;
          }
          case 'X': {
            // Hexadecimal integer, uppercase.
            numarg_types[arg_value].kind = K_UINT;
            break;
          }
          case 'f': {
            // Decimal floating point, lowercase.
            numarg_types[arg_value].kind = K_FLOAT;
            break;
          }
          case 'F': {
            // Decimal floating point, uppercase.
            numarg_types[arg_value].kind = K_FLOAT;
            break;
          }
          case 'e': {
            // Decimal floating point, lowercase, exponential notation.
            numarg_types[arg_value].kind = K_FLOAT;
            break;
          }
          case 'E': {
            // Decimal floating point, uppercase, exponential notation.
            numarg_types[arg_value].kind = K_FLOAT;
            break;
          }
          case 'g': {
            // Decimal floating point, lowercase, variable exponential notation.
            numarg_types[arg_value].kind = K_FLOAT;
            break;
          }
          case 'G': {
            // Decimal floating point, uppercase, variable exponential notation.
            numarg_types[arg_value].kind = K_FLOAT;
            break;
          }
          case 'a': {
            // Hexadecimal floating point, lowercase.
            numarg_types[arg_value].kind = K_FLOAT;
            break;
          }
          case 'A': {
            // Hexadecimal floating point, uppercase.
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
          case 's': {
            // String.
            numarg_types[arg_value].kind = K_POINTER;
            break;
          }
          case 'p': {
            // Pointer.
            numarg_types[arg_value].kind = K_POINTER;
            break;
          }
          case 'C': {
            // Wide character.
            numarg_types[arg_value].kind = K_SINT;
            numarg_types[arg_value].modifier = LM_WCHAR;
            break;
          }
          case 'S': {
            // Wide string.
            numarg_types[arg_value].kind = K_POINTER;
            break;
          }
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

static char *format_uint(uintmax_t value, unsigned int base, bool uppercase,
                         locale_t locale, char buf[static NUMBUF_SIZE]) {
  // TODO(edje): Add thousands separator.
  const char *symbols = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
  char *ret = buf + NUMBUF_SIZE;
  do {
    *--ret = symbols[value % base];
    value /= base;
  } while (value > 0);
  return ret;
}

static char *format_float2(const struct float2 *value, bool uppercase,
                           locale_t locale, char buf[static NUMBUF_SIZE]) {
  char *ret = buf + NUMBUF_SIZE;
  if (value->significand[0] == 0 && value->significand[1] == 0) {
    // Significand is zero, so ignore the exponent.
    *--ret = '0';
    *--ret = '+';
    *--ret = 'p';
    *--ret = '0';
  } else {
    // Significand is non-zero, so the number will start with a "1".
    // First print the exponent.
    int exponent = value->exponent + 127;
    exponent = exponent < 0 ? -exponent : exponent;
    do {
      *--ret = (exponent % 10) + '0';
      exponent /= 10;
    } while (exponent > 0);
    *--ret = (value->exponent + 127) < 0 ? '-' : '+';
    *--ret = 'p';

    // Print the significand. The significand will always have the form
    // 1.[digits], so just strip off the top bit to get the digits to
    // align properly.
    assert((value->significand[0] & 0x8000000000000000) != 0 &&
           "Significand is not normalized");
    uint64_t significand[2] = {
        (value->significand[0] << 1) | (value->significand[1] >> 63),
        value->significand[1] << 1,
    };
    if (significand[0] != 0 || significand[1] != 0) {
      // Skip trailing zero digits.
      while ((significand[1] & 0xf) == 0) {
        significand[1] = (significand[1] >> 4) | (significand[0] << 60);
        significand[0] >>= 4;
      }

      // Print the digits.
      const char *symbols = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
      while (significand[0] != 0 || significand[1] != 0) {
        *--ret = symbols[(significand[1] & 0xf) % 16];
        significand[1] = (significand[1] >> 4) | (significand[0] << 60);
        significand[0] >>= 4;
      }

      // TODO(edje): Use proper radix character?
      *--ret = '.';
    }
    *--ret = '1';
  }
  return ret;
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
      ssize_t l = write(fd, result, sizeof(result));                    \
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
  flockfile(stream);
#if WIDE
#define PUTCHAR(c)                   \
  do {                               \
    if (fputwc(c, stream) == WEOF) { \
      funlockfile(stream);           \
      return -1;                     \
    }                                \
    ++resultwritten;                 \
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
    if (resultstored + 1 < n) \
      s[resultstored] = (c);  \
    ++resultstored;           \
  } while (0)
#else
#error "Unknown style"
#endif

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
#define PARSE_ARGNUM(field)             \
  size_t field = get_numarg(&format);   \
  if (field == 0 || field > numarg_max) \
    goto done;                          \
  --field;
#define GET_ARG_SINT_T(type, index) ((type)numarg_values[index].v_sint)
#define GET_ARG_UINT_T(type, index) ((type)numarg_values[index].v_uint)
#define GET_ARG_POINTER_T(type, index) \
  ((const type *)numarg_values[index].v_pointer)
#define GET_ARG_FLOAT_T(type, index) ((type)numarg_values[index].v_float)
#include "vprintf_body.h"
#undef PARSE_ARGNUM
#undef GET_ARG_SINT_T
#undef GET_ARG_UINT_T
#undef GET_ARG_POINTER_T
#undef GET_ARG_FLOAT_T
  } else {
#define PARSE_ARGNUM(field)
#define GET_ARG_SINT_T(type, index) va_arg(ap, type)
#define GET_ARG_UINT_T(type, index) va_arg(ap, type)
#define GET_ARG_POINTER_T(type, index) va_arg(ap, const type *)
#define GET_ARG_FLOAT_T(type, index) va_arg(ap, type)
#include "vprintf_body.h"
#undef PARSE_ARGNUM
#undef GET_ARG_SINT_T
#undef GET_ARG_UINT_T
#undef GET_ARG_POINTER_T
#undef GET_ARG_FLOAT_T
  }
#if STYLE == VASPRINTF
done:
  // Nul-terminate the buffer.
  PUTCHAR('\0');
  *s = result;
  return resultstored - 1;
bad:
  free(result);
  return -1;
#elif STYLE == VDPRINTF
done:
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
done:
  funlockfile(stream);
  return resultwritten;
bad:
  funlockfile(stream);
  return -1;
#elif STYLE == VSNPRINTF
done:
  // Nul-terminate the buffer, if provided.
  if (n > 0)
    s[resultstored < n - 1 ? resultstored : n - 1] = '\0';
  return resultstored;
bad:
  return -1;
#endif
}
