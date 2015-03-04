// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <time.h>

size_t strftime(char *restrict s, size_t maxsize, const char *restrict format,
                const struct tm *restrict timeptr) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_TIME_MASK);
  return strftime_l(s, maxsize, format, timeptr, locale);
}
