// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <wchar.h>

size_t wcsxfrm(wchar_t *restrict ws1, const wchar_t *restrict ws2, size_t n) {
  DEFAULT_LOCALE(locale, LC_COLLATE_MASK);
  return wcsxfrm_l(ws1, ws2, n, locale);
}
