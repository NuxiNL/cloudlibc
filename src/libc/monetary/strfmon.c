// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <monetary.h>
#include <stdarg.h>

ssize_t strfmon(char *restrict s, size_t maxsize, const char *restrict format,
                ...) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_MONETARY_MASK);
  va_list ap;
  va_start(ap, format);
  ssize_t result = vstrfmon_l(s, maxsize, locale, format, ap);
  va_end(ap);
  return result;
}
