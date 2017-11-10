// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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
