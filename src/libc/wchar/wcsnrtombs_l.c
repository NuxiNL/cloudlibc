// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <wchar.h>

size_t wcsnrtombs_l(char *restrict dst, const wchar_t **restrict src,
                    size_t nwc, size_t len, mbstate_t *restrict ps,
                    locale_t locale) {
  const struct lc_ctype *ctype = locale->ctype;
  return ctype->c32stombs(dst, (const char32_t **)src, nwc, len, ctype->data);
}
