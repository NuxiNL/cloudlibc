// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <time.h>

char *strptime(const char *restrict buf, const char *restrict format,
               struct tm *restrict tm) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_TIME_MASK);
  return strptime_l(buf, format, tm, locale);
}
