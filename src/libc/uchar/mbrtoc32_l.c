// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stddef.h>
#include <uchar.h>

size_t mbrtoc32_l(char32_t *restrict pc32, const char *restrict s, size_t n,
                  mbstate_t *restrict ps, locale_t locale) {
  char32_t c32;
  if (s == NULL) {
    // No input buffer.
    pc32 = &c32;
    s = "";
    n = 1;
  } else if (pc32 == NULL) {
    // Discard character.
    pc32 = &c32;
  }

  const struct lc_ctype *ctype = locale->ctype;
  ssize_t l = ctype->mbtoc32(pc32, s, n, ps, ctype->data);
  if (l >= 0 && *pc32 == U'\0')
    return 0;
  return l;
}
