// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <wchar.h>

double wcstod(const wchar_t *restrict nptr, wchar_t **restrict endptr) {
  DEFAULT_LOCALE(locale, LC_NUMERIC_MASK);
  return wcstod_l(nptr, endptr, locale);
}
