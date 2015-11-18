// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/types.h>

#include <stdarg.h>
#include <wchar.h>

ssize_t wcsfmon_l(wchar_t *restrict s, size_t maxsize, locale_t locale,
                  const wchar_t *restrict format, ...) {
  va_list ap;
  va_start(ap, format);
  ssize_t result = vwcsfmon_l(s, maxsize, locale, format, ap);
  va_end(ap);
  return result;
}
