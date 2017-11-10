// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/mbstate.h>

#include <wchar.h>

size_t wcsnrtombs_l(char *restrict dst, const wchar_t **restrict src,
                    size_t nwc, size_t len, mbstate_t *restrict ps,
                    locale_t locale) {
  const struct lc_ctype *ctype = locale->ctype;
  ssize_t l =
      ctype->c32stombs(dst, len, (const char32_t **)src, nwc, ctype->data);
  if (l >= 0)
    mbstate_set_init(ps);
  return l;
}
