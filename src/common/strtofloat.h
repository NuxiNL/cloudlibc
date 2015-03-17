// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/float.h>
#include <common/locale.h>

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>

#if WIDE
typedef wchar_t char_t;
#else
typedef char char_t;
#endif

flt_t NAME(const char_t *restrict nptr, char_t **restrict endptr,
           locale_t locale) {
  const char_t *s = nptr;

#define PEEK(n) s[n]
#define SKIP(n) \
  do {          \
    s += (n);   \
  } while (0)
#include "parser_whitespace.h"
#include "parser_strtofloat.h"
#undef PEEK
#undef SKIP

  if (endptr != NULL)
    *endptr = (char_t *)(have_number ? s : nptr);
  if (!have_number) {
    errno = EINVAL;
    return 0;
  }
  return number;
}
