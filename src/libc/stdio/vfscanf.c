// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <stdio.h>

int vfscanf(FILE *restrict stream, const char *restrict format, va_list arg) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_NUMERIC_MASK);
  return vfscanf_l(stream, locale, format, arg);
}
