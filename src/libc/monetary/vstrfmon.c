// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <monetary.h>
#include <stdarg.h>

ssize_t vstrfmon(char *restrict s, size_t maxsize, const char *restrict format,
                 va_list ap) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_MONETARY_MASK);
  return vstrfmon_l(s, maxsize, locale, format, ap);
}
