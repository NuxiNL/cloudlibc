// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <regex.h>
#include <string.h>

int regcomp(regex_t *restrict preg, const char *restrict pattern, int cflags) {
  DEFAULT_LOCALE(locale, LC_COLLATE_MASK | LC_CTYPE_MASK);
  return regncomp_l(preg, pattern, strlen(pattern), cflags, locale);
}
