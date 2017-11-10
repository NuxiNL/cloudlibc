// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <time.h>

char *strptime(const char *restrict buf, const char *restrict format,
               struct tm *restrict tm) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_TIME_MASK);
  return strptime_l(buf, format, tm, locale);
}
