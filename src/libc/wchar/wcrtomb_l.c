// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uchar.h>
#include <wchar.h>

size_t wcrtomb_l(char *restrict s, wchar_t wc, mbstate_t *restrict ps,
                 locale_t locale) {
  return c32rtomb_l(s, wc, ps, locale);
}
