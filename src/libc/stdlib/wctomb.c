// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdlib.h>

int wctomb(char *s, wchar_t wchar) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return wctomb_l(s, wchar, locale);
}
