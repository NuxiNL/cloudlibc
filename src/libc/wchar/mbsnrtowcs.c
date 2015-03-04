// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <wchar.h>

size_t mbsnrtowcs(wchar_t *restrict dst, const char **restrict src, size_t nmc,
                  size_t len, mbstate_t *restrict ps) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return mbsnrtowcs_l(dst, src, nmc, len, ps, locale);
}
