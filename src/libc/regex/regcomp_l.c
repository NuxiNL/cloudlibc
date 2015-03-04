// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <locale.h>
#include <regex.h>
#include <string.h>

int regcomp_l(regex_t *restrict preg, const char *restrict pattern, int cflags,
              locale_t locale) {
  return regncomp_l(preg, pattern, strlen(pattern), cflags, locale);
}
