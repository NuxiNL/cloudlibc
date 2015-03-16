// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>

size_t mbstowcs_l(wchar_t *restrict pwcs, const char *restrict s, size_t n,
                  locale_t locale) {
  const char *src = s;
  mbstate_t ps = {};
  return mbsnrtowcs_l(pwcs, &src, SIZE_MAX, n, &ps, locale);
}
