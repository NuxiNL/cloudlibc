// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <monetary.h>
#include <stdarg.h>

ssize_t strfmon_l(char *restrict s, size_t maxsize, locale_t locale,
                  const char *restrict format, ...) {
  va_list ap;
  va_start(ap, format);
  ssize_t result = vstrfmon_l(s, maxsize, locale, format, ap);
  va_end(ap);
  return result;
}
