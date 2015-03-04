// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

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
