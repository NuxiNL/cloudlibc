// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/mbstate.h>

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
  ssize_t l = ctype->c32tomb(s, c32, ctype->data);
  if (l >= 0)
    mbstate_set_init(ps);
  return l;
}
