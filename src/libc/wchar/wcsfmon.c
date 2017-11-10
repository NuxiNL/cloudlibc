// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <sys/types.h>

#include <locale.h>
#include <stdarg.h>
#include <wchar.h>

ssize_t wcsfmon(wchar_t *restrict s, size_t maxsize,
                const wchar_t *restrict format, ...) {
  DEFAULT_LOCALE(locale, LC_MONETARY_MASK);
  va_list ap;
  va_start(ap, format);
  ssize_t result = vwcsfmon_l(s, maxsize, locale, format, ap);
  va_end(ap);
  return result;
}
