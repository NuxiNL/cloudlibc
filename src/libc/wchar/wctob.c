// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <wchar.h>

int wctob(wint_t c) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return wctob_l(c, locale);
}
