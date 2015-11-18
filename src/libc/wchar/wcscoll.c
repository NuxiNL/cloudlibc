// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <wchar.h>

int wcscoll(const wchar_t *ws1, const wchar_t *ws2) {
  DEFAULT_LOCALE(locale, LC_COLLATE_MASK | LC_CTYPE_MASK);
  return wcscoll_l(ws1, ws2, locale);
}
