// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uchar.h>
#include <wchar.h>

size_t mbrtowc_l(wchar_t *restrict pwc, const char *restrict s, size_t n,
                 mbstate_t *restrict ps, locale_t locale) {
  return mbrtoc32_l((char32_t *)pwc, s, n, ps, locale);
}
