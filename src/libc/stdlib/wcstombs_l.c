// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>

size_t wcstombs_l(char *restrict s, const wchar_t *restrict pwcs, size_t n,
                  locale_t locale) {
  const wchar_t *src = pwcs;
  mbstate_t ps = {};
  return wcsnrtombs_l(s, &src, SIZE_MAX, n, &ps, locale);
}
