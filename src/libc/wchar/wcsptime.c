// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <wchar.h>

wchar_t *wcsptime(const wchar_t *restrict buf, const wchar_t *restrict format,
                  struct tm *restrict tm) {
  DEFAULT_LOCALE(locale, LC_TIME_MASK);
  return wcsptime_l(buf, format, tm, locale);
}
