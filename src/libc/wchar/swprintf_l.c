// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdarg.h>
#include <wchar.h>

int swprintf_l(wchar_t *restrict ws, size_t n, locale_t locale,
               const wchar_t *restrict format, ...) {
  va_list ap;
  va_start(ap, format);
  int result = vswprintf_l(ws, n, locale, format, ap);
  va_end(ap);
  return result;
}
