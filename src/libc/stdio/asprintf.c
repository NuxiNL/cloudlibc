// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdarg.h>
#include <stdio.h>
#include <cloudlibc_interceptors.h>

int __cloudlibc_asprintf(char **s, const char *format, ...) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_NUMERIC_MASK);
  va_list ap;
  va_start(ap, format);
  int result = vasprintf_l(s, locale, format, ap);
  va_end(ap);
  return result;
}

__weak_reference(__cloudlibc_asprintf, asprintf);
