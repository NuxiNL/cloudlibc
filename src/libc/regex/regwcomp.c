// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <regex.h>
#include <wchar.h>

int regwcomp(regex_t *restrict preg, const wchar_t *restrict pattern,
             int cflags) {
  DEFAULT_LOCALE(locale, LC_COLLATE_MASK | LC_CTYPE_MASK);
  return regwncomp_l(preg, pattern, wcslen(pattern), cflags, locale);
}
