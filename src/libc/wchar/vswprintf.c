// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <wchar.h>

int vswprintf(wchar_t *restrict ws, size_t n, const wchar_t *restrict format,
              va_list arg) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_NUMERIC_MASK);
  return vswprintf_l(ws, n, locale, format, arg);
}
