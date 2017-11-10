// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stddef.h>
#include <stdint.h>

static size_t get_number(const char_t *restrict *format) {
  const char_t *s = *format;
  if (*s < '0' || *s > '9')
    return 0;
  int value = 0;
  do {
    value = value * 10 + *s++ - '0';
  } while (*s >= '0' && *s <= '9');
  *format = s;
  return value;
}

// Parses the "n$" part of a numerical argument reference of a format
// string. Upon success, this function returns a number greater than
// zero indicating the index of the numerical argument. Upon failure it
// returns zero.
static size_t get_numarg(const char_t *restrict *format) {
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

enum length_modifier {
  LM_SHORT_SHORT,
  LM_SHORT,
  LM_DEFAULT,
  LM_LONG,
  LM_LONG_LONG,
  LM_LONG_DOUBLE
};

#define LM_INFERRED(type)                     \
  _Generic((type)0, signed char               \
           : LM_SHORT_SHORT, unsigned char    \
           : LM_SHORT_SHORT, signed short     \
           : LM_SHORT, unsigned short         \
           : LM_SHORT, signed int             \
           : LM_DEFAULT, unsigned int         \
           : LM_DEFAULT, signed long          \
           : LM_LONG, unsigned long           \
           : LM_LONG, signed long long        \
           : LM_LONG_LONG, unsigned long long \
           : LM_LONG_LONG)

#define LM_CHAR LM_INFERRED(unsigned char)
#define LM_MAX LM_INFERRED(intmax_t)
#define LM_PTRDIFF LM_INFERRED(ptrdiff_t)
#define LM_SIZE LM_INFERRED(size_t)
#define LM_WCHAR LM_INFERRED(wchar_t)

static enum length_modifier get_length_modifier(
    const char_t *restrict *format) {
  if ((*format)[0] == 'h' && (*format)[1] == 'h') {
    *format += 2;
    return LM_SHORT_SHORT;
  } else if ((*format)[0] == 'h') {
    ++*format;
    return LM_SHORT;
  } else if ((*format)[0] == 'l' && (*format)[1] == 'l') {
    *format += 2;
    return LM_LONG_LONG;
  } else if ((*format)[0] == 'l') {
    ++*format;
    return LM_LONG;
  } else if ((*format)[0] == 'j') {
    ++*format;
    return LM_MAX;
  } else if ((*format)[0] == 'z') {
    ++*format;
    return LM_SIZE;
  } else if ((*format)[0] == 't') {
    ++*format;
    return LM_PTRDIFF;
  } else if ((*format)[0] == 'L') {
    ++*format;
    return LM_LONG_DOUBLE;
  }
  return LM_DEFAULT;
}
