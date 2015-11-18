// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>

size_t wcstombs(char *restrict s, const wchar_t *restrict pwcs, size_t n) {
  const wchar_t *src = pwcs;
  mbstate_t ps = {};
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return wcsnrtombs_l(s, &src, SIZE_MAX, n, &ps, locale);
}
