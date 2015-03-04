// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <regex.h>

int regncomp(regex_t *restrict preg, const char *restrict pattern, size_t len,
             int cflags) {
  DEFAULT_LOCALE(locale, LC_COLLATE_MASK | LC_CTYPE_MASK);
  return regncomp_l(preg, pattern, len, cflags, locale);
}
