// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <time.h>

size_t strftime(char *restrict s, size_t maxsize, const char *restrict format,
                const struct tm *restrict timeptr) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_TIME_MASK);
  return strftime_l(s, maxsize, format, timeptr, locale);
}
