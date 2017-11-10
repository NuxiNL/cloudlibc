// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <stdarg.h>
#include <stdio.h>

int fscanf_l(FILE *restrict stream, locale_t locale,
             const char *restrict format, ...) {
  va_list ap;
  va_start(ap, format);
  int result = vfscanf_l(stream, locale, format, ap);
  va_end(ap);
  return result;
}
