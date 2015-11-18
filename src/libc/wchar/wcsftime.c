// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <wchar.h>

size_t wcsftime(wchar_t *restrict wcs, size_t maxsize,
                const wchar_t *restrict format,
                const struct tm *restrict timeptr) {
  DEFAULT_LOCALE(locale, LC_TIME_MASK);
  return wcsftime_l(wcs, maxsize, format, timeptr, locale);
}
