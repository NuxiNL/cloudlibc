// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/mbstate.h>

#include <locale.h>
#include <stdlib.h>
#include <uchar.h>

int mbtowc_l(wchar_t *restrict pwc, const char *restrict s, size_t n,
             locale_t locale) {
  // This implementation does not support any state-dependent encodings.
  if (s == NULL)
    return 0;

  const struct lc_ctype *ctype = locale->ctype;
  char32_t c32;
  mbstate_t ps;
  mbstate_set_init(&ps);
  ssize_t l = ctype->mbtoc32(&c32, s, n, &ps, ctype->data);
  if (l < 0) {
    // We must yield a full character.
    errno = EILSEQ;
    return -1;
  }
  if (pwc != NULL)
    *pwc = c32;
  return c32 == U'\0' ? 0 : l;
}
