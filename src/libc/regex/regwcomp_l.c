// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <regex.h>
#include <wchar.h>

int regwcomp_l(regex_t *restrict preg, const wchar_t *restrict pattern,
               int cflags, locale_t locale) {
  return regwncomp_l(preg, pattern, wcslen(pattern), cflags, locale);
}
