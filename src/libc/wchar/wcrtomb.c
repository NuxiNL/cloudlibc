// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <uchar.h>
#include <wchar.h>

size_t wcrtomb(char *restrict s, wchar_t wc, mbstate_t *restrict ps) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return c32rtomb_l(s, wc, ps, locale);
}
