// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <locale.h>
#include <regex.h>
#include <wchar.h>

int regwcomp_l(regex_t *restrict preg, const wchar_t *restrict pattern,
               int cflags, locale_t locale) {
  return regwncomp_l(preg, pattern, wcslen(pattern), cflags, locale);
}
