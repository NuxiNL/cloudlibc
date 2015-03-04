// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uchar.h>
#include <wchar.h>

size_t wcrtomb_l(char *restrict s, wchar_t wc, mbstate_t *restrict ps,
                 locale_t locale) {
  return c32rtomb_l(s, wc, ps, locale);
}
