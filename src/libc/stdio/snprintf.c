// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdarg.h>
#include <stdio.h>
#include <cloudlibc_interceptors.h>

int __cloudlibc_snprintf(char *restrict s, size_t n, const char *restrict format, ...) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_NUMERIC_MASK);
  va_list ap;
  va_start(ap, format);
  int result = vsnprintf_l(s, n, locale, format, ap);
  va_end(ap);
  return result;
}

__weak_reference(__cloudlibc_snprintf, snprintf);
