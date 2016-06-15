// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <stdlib.h>

int wctomb(char *s, wchar_t wchar) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return wctomb_l(s, wchar, locale);
}
