// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <uchar.h>
#include <wchar.h>

size_t mbrtowc(wchar_t *restrict pwc, const char *restrict s, size_t n,
               mbstate_t *restrict ps) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return mbrtoc32_l((char32_t *)pwc, s, n, ps, locale);
}
