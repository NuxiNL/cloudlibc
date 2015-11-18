// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdarg.h>
#include <wchar.h>

int fwprintf_l(FILE *restrict stream, locale_t locale,
               const wchar_t *restrict format, ...) {
  va_list ap;
  va_start(ap, format);
  int result = vfwprintf_l(stream, locale, format, ap);
  va_end(ap);
  return result;
}
