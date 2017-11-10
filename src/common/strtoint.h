// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/limits.h>
#include <common/locale.h>
#include <common/mbstate.h>

#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>

#if WIDE
typedef wchar_t char_t;
#else
typedef char char_t;
#endif

#if WIDE
int_t NAME(const char_t *restrict str, char_t **restrict endptr, int base) {
#else
int_t NAME(const char_t *restrict str, char_t **restrict endptr, int base,
           locale_t locale) {
#endif
  const char_t *s = str;
  int_t min = NUMERIC_MIN(int_t);
  int_t max = NUMERIC_MAX(int_t);
  bool allow_negative = true;

#define PEEK(n) s[n]
#define SKIP(n) \
  do {          \
    s += (n);   \
  } while (0)
#include "parser_whitespace.h"

#include "parser_strtoint.h"
#undef PEEK
#undef SKIP

  if (!have_number)
    errno = EINVAL;
  else if (have_overflow)
    errno = ERANGE;
  if (endptr != NULL)
    *endptr = (char_t *)(have_number ? s : str);
  return number;
}
