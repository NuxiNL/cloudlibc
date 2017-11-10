// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <wchar.h>

size_t mbsnrtowcs(wchar_t *restrict dst, const char **restrict src, size_t nmc,
                  size_t len, mbstate_t *restrict ps) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return mbsnrtowcs_l(dst, src, nmc, len, ps, locale);
}
