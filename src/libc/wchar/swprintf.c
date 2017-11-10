// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdarg.h>
#include <wchar.h>

int swprintf(wchar_t *restrict ws, size_t n, const wchar_t *restrict format,
             ...) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_NUMERIC_MASK);
  va_list ap;
  va_start(ap, format);
  int result = vswprintf_l(ws, n, locale, format, ap);
  va_end(ap);
  return result;
}
