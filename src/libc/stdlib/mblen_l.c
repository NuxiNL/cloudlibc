// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <locale.h>
#include <stdlib.h>

int mblen_l(const char *s, size_t n, locale_t locale) {
  return mbtowc_l(NULL, s, n, locale);
}
