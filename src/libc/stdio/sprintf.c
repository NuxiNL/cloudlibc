// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <common/locale.h>

#include <locale.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

int sprintf(char *restrict s, const char *restrict format, ...) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_NUMERIC_MASK);
  va_list ap;
  va_start(ap, format);
  int result = vsnprintf_l(s, SIZE_MAX, locale, format, ap);
  va_end(ap);
  return result;
}
