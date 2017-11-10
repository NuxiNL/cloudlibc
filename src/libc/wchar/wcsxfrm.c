// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <wchar.h>

size_t wcsxfrm(wchar_t *restrict ws1, const wchar_t *restrict ws2, size_t n) {
  DEFAULT_LOCALE(locale, LC_COLLATE_MASK);
  return wcsxfrm_l(ws1, ws2, n, locale);
}
