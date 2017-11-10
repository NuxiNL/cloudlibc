// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/byteset.h>
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

  // Freelist of objects that are allocated by this function and need to
  // be freed in case this function doesn't terminate successfully.
  void **freelist = NULL;
  size_t freelistused = 0;
  size_t freelistlen = 0;
#define FREELIST_APPEND(ptr)                                         \
  do {                                                               \
    if (freelistused + 1 > freelistlen) {                            \
      size_t newfreelistlen = freelistlen < 4 ? 4 : freelistlen * 2; \
      void **newfreelist =                                           \
          reallocarray(freelist, newfreelistlen, sizeof(void *));    \
      if (newfreelist == NULL) {                                     \
        free(ptr);                                                   \
        goto bad;                                                    \
      }                                                              \
      freelist = newfreelist;                                        \
      freelistlen = newfreelistlen;                                  \
    }                                                                \
    freelist[freelistused++] = (ptr);                                \
  } while (0)

  // Copy of the arguments list, used for looking up numbered arguments.
  va_list numargs;
  va_copy(numargs, ap);

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
          goto done;
        INPUT_SKIP(idx + 1);
        continue;
      }

      // Field number, in case of numbered arguments.
      size_t numarg = get_numarg(&format);

      // Suppress assignment.
      bool suppress = false;
      if (*format == '*') {
        suppress = true;
        ++format;
      }
#define CONVERSION_PERFORMED() \
  do {                         \
    if (!suppress)             \
      ++conversions_performed; \
  } while (0)

      // Fetch the appropriate argument value.
      void *argument;
      if (!suppress) {
        if (numarg > 0) {
          // Numbered arguments. Fetch the argument at the right index.
          va_list n;
          va_copy(n, numargs);
          while (numarg-- > 0)
            argument = va_arg(n, void *);
          va_end(n);
        } else {
          // Non-numbered argument. Simply fetch the next argument.
          argument = va_arg(ap, void *);
        }
      }

// Assigns a value to an integer argument, taking the size of the
// argument into consideration.
#define ARGUMENT_SET_INT(value)             \
  do {                                      \
    if (!suppress) {                        \
      switch (length) {                     \
        case LM_SHORT_SHORT:                \
          *(char *)argument = (value);      \
          break;                            \
        case LM_SHORT:                      \
          *(short *)argument = (value);     \
          break;                            \
        case LM_LONG:                       \
          *(long *)argument = (value);      \
          break;                            \
        case LM_LONG_LONG:                  \
          *(long long *)argument = (value); \
          break;                            \
        default:                            \
          *(int *)argument = (value);       \
          break;                            \
      }                                     \
    }                                       \
  } while (0)

// Prepare for writing string output into an argument, either by
// providing a buffer or using the assignment-allocation flag 'm'.
// Use SIZE_MAX as a buffer size for non-allocated buffers to prevent
// ARGUMENT_STR_APPEND() from reallocating.
//
// TODO(ed): Add support for wide/narrow string conversion.
#define ARGUMENT_STR_START() \
  char_t *out;               \
  size_t outlen;             \
  size_t outused;            \
  do {                       \
    if (!suppress) {         \
      if (allocate) {        \
        out = NULL;          \
        outlen = 0;          \
      } else {               \
        out = argument;      \
        outlen = SIZE_MAX;   \
      }                      \
      outused = 0;           \
    }                        \
  } while (0)

// Appends a character to a string argument buffer.
#define ARGUMENT_STR_APPEND(c)                                         \
  do {                                                                 \
    if (!suppress) {                                                   \
      if (outused + 2 > outlen) {                                      \
        size_t newoutlen = outlen < 16 ? 16 : outlen * 2;              \
        char_t *newout = reallocarray(out, newoutlen, sizeof(char_t)); \
        if (newout == NULL) {                                          \
          free(out);                                                   \
          goto bad;                                                    \
        }                                                              \
        out = newout;                                                  \
        outlen = newoutlen;                                            \
      }                                                                \
      out[outused++] = (c);                                            \
    }                                                                  \
  } while (0)

// Finalize writing the string argument, either null terminating it or
// leaving it unterminated.
#define ARGUMENT_STR_FINISH_NULL() \
  do {                             \
    if (!suppress)                 \
      out[outused] = '\0';         \
    ARGUMENT_STR_FINISH_NONULL();  \
  } while (0)
#define ARGUMENT_STR_FINISH_NONULL() \
  do {                               \
    if (!suppress && allocate) {     \
      FREELIST_APPEND(out);          \
      *(char_t **)argument = out;    \
    }                                \
    CONVERSION_PERFORMED();          \
  } while (0)

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
          bool allow_negative = true;
#include "parser_strtoint.h"
#undef PEEK
#undef SKIP

          if (!have_number)
            goto done;
          INPUT_SKIP(idx);
          ARGUMENT_SET_INT(number);
          CONVERSION_PERFORMED();
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
          bool allow_negative = true;
#include "parser_strtoint.h"
#undef PEEK
#undef SKIP

          if (!have_number)
            goto done;
          INPUT_SKIP(idx);
          ARGUMENT_SET_INT(number);
          CONVERSION_PERFORMED();
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

          // Parse floating point number, taking the limits of the
          // requested type into account.
          size_t end = field_width == 0 ? SIZE_MAX : idx + field_width;
#define PEEK(n)                                                              \
  (idx + (n) < end && INPUT_REMAINING(idx + (n) + 1) ? INPUT_PEEK(idx + (n)) \
                                                     : '\0')
          switch (length) {
            case LM_LONG: {
              typedef double flt_t;
#include "parser_strtofloat.h"
              if (!have_number)
                goto done;
              if (!suppress)
                *(double *)argument = number;
              break;
            }
            case LM_LONG_DOUBLE: {
              typedef long double flt_t;
#include "parser_strtofloat.h"
              if (!have_number)
                goto done;
              if (!suppress)
                *(long double *)argument = number;
              break;
            }
            default: {
              typedef float flt_t;
#include "parser_strtofloat.h"
              if (!have_number)
                goto done;
              if (!suppress)
                *(float *)argument = number;
              break;
            }
          }
#undef PEEK
#undef SKIP
          INPUT_SKIP(idx);
          CONVERSION_PERFORMED();
          break;
        }

        case 'S':
          // Wide string.
          length = LM_LONG;
        case 's': {
          // String.

          // Skip leading whitespace.
          size_t idx = 0;
#define SKIP(n) \
  do {          \
    idx += (n); \
  } while (0)
#define PEEK(n) (INPUT_REMAINING(idx + (n) + 1) ? INPUT_PEEK(idx + (n)) : '\0')
#include "parser_whitespace.h"
#undef PEEK
#undef SKIP

          if (field_width == 0)
            field_width = SIZE_MAX;
          size_t i = 0;
          ARGUMENT_STR_START();
          for (;;) {
#if WIDE
            // Respect field width.
            if (i >= field_width)
              break;
            // Fetch next character.
            if (!INPUT_REMAINING(idx + i + 1))
              goto string_end;
            char_t c = INPUT_PEEK(idx + i);
            // Terminate if whitespace.
            if (iswspace(c))
              goto string_end;
            // Append character to output.
            ARGUMENT_STR_APPEND(c);
            ++i;
#else
            const struct lc_ctype *ctype = locale->ctype;
            mbstate_t mbs;
            mbstate_set_init(&mbs);
            size_t peeklen = 0;
            for (;;) {
              // Respect field width.
              if (i + peeklen >= field_width)
                break;
              // Fetch next byte.
              if (!INPUT_REMAINING(idx + i + peeklen + 1))
                goto string_end;
              char_t b = INPUT_PEEK(idx + i + peeklen);
              ++peeklen;
              char32_t c32;
              ssize_t ret = ctype->mbtoc32(&c32, &b, 1, &mbs, ctype->data);
              if (ret != -2) {
                // Multibyte sequence complete. Terminate if whitespace.
                if (ret >= 0 && iswspace(c32))
                  goto string_end;
                break;
              }
            }
            // Append multibyte sequence to output.
            for (size_t j = 0; j < peeklen; ++j)
              ARGUMENT_STR_APPEND(INPUT_PEEK(idx + i + j));
            i += peeklen;
#endif
          }
        string_end:
          if (i == 0)
            goto done;
          ARGUMENT_STR_FINISH_NULL();
          INPUT_SKIP(idx + i);
          break;
        }

        case '[': {
#if WIDE
          // For wide characters, we cannot (easily) use linear-time
          // string matching. Store the start of the scanset and skip
          // past it.
          const char_t *scanset = format;
          if (*format == '^')
            ++format;
          do {
            ++format;
          } while (*format != ']');
#else
          // Convert provided list of characters into a bitmask, so we
          // can perform O(m+n) string matching, like strcspn() and
          // strspn().
          byteset_t bs;
          if (*format == '^') {
            // Leading circumflex: negative matching.
            ++format;
            bytefillset(&bs);
            do {
              bytedelset(&bs, *format++);
            } while (*format != ']');
          } else {
            // No leading circumflex: positive matching.
            byteemptyset(&bs);
            do {
              byteaddset(&bs, *format++);
            } while (*format != ']');
          }
#endif
          ++format;

          // Get sequence from input, only allowing characters from the
          // provided set.
          if (field_width == 0)
            field_width = SIZE_MAX;
          size_t i = 0;
          ARGUMENT_STR_START();
          do {
            if (!INPUT_REMAINING(i + 1))
              break;
            char_t c = INPUT_PEEK(i);
#if WIDE
            const char_t *ss = scanset;
            if (*ss == '^') {
              // Leading circumflex: negative matching.
              ++ss;
              do {
                if (*ss++ == c)
                  goto scanset_mismatch;
              } while (*ss != ']');
            } else {
              // No leading circumflex: positive matching.
              while (*ss != c) {
                if (*++ss == ']')
                  goto scanset_mismatch;
              }
            }
#else
            if (!byteismember(&bs, c))
              goto scanset_mismatch;
#endif
            ARGUMENT_STR_APPEND(c);
            ++i;
          } while (i < field_width);
        scanset_mismatch:
          if (i == 0)
            goto done;
          ARGUMENT_STR_FINISH_NULL();
          INPUT_SKIP(i);
          break;
        }

        case 'C':
          // Fixed-size string of wide characters.
          length = LM_LONG;
        case 'c': {
          // Fixed-size string of characters.
          if (field_width == 0)
            field_width = 1;
          if (!INPUT_REMAINING(field_width))
            goto done;
          ARGUMENT_STR_START();
          for (size_t i = 0; i < field_width; ++i)
            ARGUMENT_STR_APPEND(INPUT_PEEK(i));
          ARGUMENT_STR_FINISH_NONULL();
          INPUT_SKIP(field_width);
          break;
        }

        case 'n': {
          // Number of characters parsed.
          ARGUMENT_SET_INT(input_read);
          break;
        }
      }
    } else {
      // See if the format contains any whitespace characters. If so,
      // trim whitespace from the input.
      bool have_whitespace = false;
#define SKIP(n)             \
  do {                      \
    format += (n);          \
    have_whitespace = true; \
  } while (0)
#define PEEK(n) format[n]
#include "parser_whitespace.h"
#undef PEEK
#undef SKIP
      if (have_whitespace) {
#define SKIP(n) INPUT_SKIP(n)
#define PEEK(n) (INPUT_REMAINING((n) + 1) ? INPUT_PEEK(n) : '\0')
#include "parser_whitespace.h"
#undef PEEK
#undef SKIP
      } else {
        // No whitespace. Perform exact match against character in
        // format string.
        if (!INPUT_REMAINING(1) || INPUT_PEEK(0) != *format++)
          goto done;
        INPUT_SKIP(1);
      }
    }
  }
done:
  // Terminate successfully. Free the freelist, but not the objects
  // contained.
  free(freelist);
  va_end(numargs);
  return conversions_performed;
bad:
  // Terminate unsuccessfully. Free the freelist and also the objects
  // contained.
  for (size_t i = 0; i < freelistused; ++i)
    free(freelist[i]);
  free(freelist);
  va_end(numargs);
  return EOF;
}
