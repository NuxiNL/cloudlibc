// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/mbstate.h>

#include <stdint.h>
#include <strings.h>
#include <wctype.h>

static wint_t fetchchar(const char **s, mbstate_t *mb, locale_t locale) {
  const struct lc_ctype *ctype = locale->ctype;
  char32_t c32;
  ssize_t l = ctype->mbtoc32(&c32, *s, SIZE_MAX, mb, ctype->data);
  // Return a negative value to force inequality against a valid string.
  if (l < 0)
    return -1;
  *s += l;
  return c32;
}

int strcasecmp_l(const char *s1, const char *s2, locale_t locale) {
  mbstate_t mb1, mb2;
  mbstate_set_init(&mb1);
  mbstate_set_init(&mb2);
  for (;;) {
    wint_t c1 = towlower(fetchchar(&s1, &mb1, locale));
    wint_t c2 = towlower(fetchchar(&s2, &mb2, locale));
    if (c1 < c2)
      return -1;
    if (c1 > c2)
      return 1;
    if (c1 <= 0)
      return 0;
  }
}
