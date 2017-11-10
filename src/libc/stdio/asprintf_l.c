// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <stdarg.h>
#include <stdio.h>

int asprintf_l(char **s, locale_t locale, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  int result = vasprintf_l(s, locale, format, ap);
  va_end(ap);
  return result;
}
