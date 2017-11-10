// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdlib.h>

int mblen(const char *s, size_t n) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return mbtowc_l(NULL, s, n, locale);
}
