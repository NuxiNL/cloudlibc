// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <strings.h>
#include <wctype.h>

static wint_t fetchchar(const char **s, size_t *n, struct mbtoc32state *mb,
                        locale_t locale) {
  const struct lc_ctype *ctype = locale->ctype;
  char32_t c32;
  ssize_t l = ctype->mbtoc32(&c32, *s, *n, mb, ctype->data);
  // Return a negative value to force inequality against a valid string.
  if (l < 0)
    return -1;
  *s += l;
  *n -= l;
  return c32;
}

int strncasecmp_l(const char *s1, const char *s2, size_t n, locale_t locale) {
  struct mbtoc32state mb1 = {}, mb2 = {};
  size_t n1 = n, n2 = n;
  for (;;) {
    wint_t c1 = towlower(fetchchar(&s1, &n1, &mb1, locale));
    wint_t c2 = towlower(fetchchar(&s2, &n2, &mb2, locale));
    if (c1 < c2)
      return -1;
    if (c1 > c2)
      return 1;
    if (c1 <= L'\0')
      return 0;
  }
}
