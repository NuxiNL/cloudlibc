// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdarg.h>
#include <wchar.h>

int fwscanf_l(FILE *restrict stream, locale_t locale,
              const wchar_t *restrict format, ...) {
  va_list ap;
  va_start(ap, format);
  int result = vfwscanf_l(stream, locale, format, ap);
  va_end(ap);
  return result;
}
