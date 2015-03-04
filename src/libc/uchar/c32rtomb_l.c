// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <limits.h>
#include <stddef.h>
#include <uchar.h>

size_t c32rtomb_l(char *restrict s, char32_t c32, mbstate_t *restrict ps,
                  locale_t locale) {
  char buf[MB_LEN_MAX];
  if (s == NULL) {
    // No output buffer.
    s = buf;
    c32 = U'\0';
  }

  const struct lc_ctype *ctype = locale->ctype;
  return ctype->c32tomb(s, c32, ctype->data);
}
