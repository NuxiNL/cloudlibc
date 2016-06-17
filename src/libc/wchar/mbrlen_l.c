// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <wchar.h>

size_t mbrlen_l(const char *restrict s, size_t n, mbstate_t *restrict ps,
                locale_t locale) {
  const struct lc_ctype *ctype = locale->ctype;
  char32_t c32;
  ssize_t l = ctype->mbtoc32(&c32, s, n, ps, ctype->data);
  if (l >= 0 && c32 == U'\0')
    return 0;
  return l;
}
