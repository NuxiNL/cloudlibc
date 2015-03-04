// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <stdio.h>

FILE *fmemopen(void *restrict buf, size_t size, const char *restrict mode) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return fmemopen_l(buf, size, mode, locale);
}
