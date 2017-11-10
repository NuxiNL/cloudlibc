// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <sys/types.h>

#include <locale.h>
#include <wchar.h>

ssize_t vwcsfmon(wchar_t *restrict s, size_t maxsize,
                 const wchar_t *restrict format, va_list ap) {
  DEFAULT_LOCALE(locale, LC_MONETARY_MASK);
  return vwcsfmon_l(s, maxsize, locale, format, ap);
}
