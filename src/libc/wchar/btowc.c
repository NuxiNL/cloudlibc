// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <wchar.h>

wint_t btowc(int c) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return btowc_l(c, locale);
}
