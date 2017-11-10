// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <regex.h>
#include <string.h>

int regcomp_l(regex_t *restrict preg, const char *restrict pattern, int cflags,
              locale_t locale) {
  return regncomp_l(preg, pattern, strlen(pattern), cflags, locale);
}
