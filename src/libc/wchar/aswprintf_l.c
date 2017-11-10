// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdarg.h>
#include <wchar.h>

int aswprintf_l(wchar_t **restrict ws, locale_t locale,
                const wchar_t *restrict format, ...) {
  va_list ap;
  va_start(ap, format);
  int result = vaswprintf_l(ws, locale, format, ap);
  va_end(ap);
  return result;
}
