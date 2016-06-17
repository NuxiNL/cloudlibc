// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>

size_t mbstowcs(wchar_t *restrict pwcs, const char *restrict s, size_t n) {
  const char *src = s;
  static const mbstate_t initial_mbstate;
  mbstate_t ps = initial_mbstate;
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return mbsnrtowcs_l(pwcs, &src, SIZE_MAX, n, &ps, locale);
}
