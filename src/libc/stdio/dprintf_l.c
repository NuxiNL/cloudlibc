// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <locale.h>
#include <stdarg.h>
#include <stdio.h>

int dprintf_l(int fildes, locale_t locale, const char *restrict format, ...) {
  va_list ap;
  va_start(ap, format);
  int result = vdprintf_l(fildes, locale, format, ap);
  va_end(ap);
  return result;
}
