// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>

size_t wcstombs_l(char *restrict s, const wchar_t *restrict pwcs, size_t n,
                  locale_t locale) {
  const wchar_t *src = pwcs;
  static const mbstate_t initial_mbstate;
  mbstate_t ps = initial_mbstate;
  return wcsnrtombs_l(s, &src, SIZE_MAX, n, &ps, locale);
}
