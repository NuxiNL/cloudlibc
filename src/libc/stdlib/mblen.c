// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <stdlib.h>

int mblen(const char *s, size_t n) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return mbtowc_l(NULL, s, n, locale);
}
