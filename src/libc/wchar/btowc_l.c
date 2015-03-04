// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <stdio.h>
#include <wchar.h>

wint_t btowc_l(int c, locale_t locale) {
  // The btowc() function shall return WEOF if c has the value EOF.
  if (c == EOF)
    return WEOF;

  const struct lc_ctype *ctype = locale->ctype;
  char32_t c32;
  char buf = c;
  struct mbtoc32state ps = {};
  if (ctype->mbtoc32(&c32, &buf, 1, &ps, ctype->data) != 1)
    return WEOF;
  return c32;
}
