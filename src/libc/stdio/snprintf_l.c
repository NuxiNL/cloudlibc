// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <stdarg.h>
#include <stdio.h>

int snprintf_l(char *restrict s, size_t n, locale_t locale,
               const char *restrict format, ...) {
  va_list ap;
  va_start(ap, format);
  int result = vsnprintf_l(s, n, locale, format, ap);
  va_end(ap);
  return result;
}
