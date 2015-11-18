// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <limits.h>
#include <stdio.h>
#include <wchar.h>

int wctob_l(wint_t c, locale_t locale) {
  const struct lc_ctype *ctype = locale->ctype;
  char buf[MB_LEN_MAX];
  if (ctype->c32tomb(buf, c, ctype->data) != 1)
    return EOF;
  return (unsigned char)buf[0];
}
