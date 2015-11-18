// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <regex.h>
#include <string.h>

int regcomp(regex_t *restrict preg, const char *restrict pattern, int cflags) {
  DEFAULT_LOCALE(locale, LC_COLLATE_MASK | LC_CTYPE_MASK);
  return regncomp_l(preg, pattern, strlen(pattern), cflags, locale);
}
