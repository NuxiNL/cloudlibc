// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <common/locale.h>

#include <locale.h>
#include <stdint.h>
#include <stdio.h>

int vsprintf(char *restrict s, const char *restrict format, va_list ap) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_NUMERIC_MASK);
  return vsnprintf_l(s, SIZE_MAX, locale, format, ap);
}
