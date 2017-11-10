// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef CTYPE_CTYPE_IMPL_H
#define CTYPE_CTYPE_IMPL_H

#include <common/locale.h>
#include <common/mbstate.h>

#include <limits.h>
#include <wctype.h>

static inline int isctype_l(int c, wctype_t wctype, locale_t locale) {
  // Bounds checking.
  if (c < 0 || c > UCHAR_MAX)
    return 0;

  // Convert to wide character.
  const struct lc_ctype *ctype = locale->ctype;
  char32_t c32;
  {
    char buf = c;
    mbstate_t ps;
    mbstate_set_init(&ps);
    if (ctype->mbtoc32(&c32, &buf, 1, &ps, ctype->data) != 1)
      return 0;
  }

  // Perform matching using wide character function.
  return wctype(c32);
}

static inline int toctrans_l(int c, wctrans_t wctrans, locale_t locale) {
  // Bounds checking.
  if (c < 0 || c > UCHAR_MAX)
    return c;

  // Convert to wide character.
  const struct lc_ctype *ctype = locale->ctype;
  char32_t c32;
  {
    char buf = c;
    mbstate_t ps;
    mbstate_set_init(&ps);
    if (ctype->mbtoc32(&c32, &buf, 1, &ps, ctype->data) != 1)
      return c;
  }

  // Convert transformed character back to single byte.
  char buf[MB_LEN_MAX];
  if (ctype->c32tomb(buf, wctrans(c32), ctype->data) != 1)
    return c;
  return (unsigned char)buf[0];
}

#endif
