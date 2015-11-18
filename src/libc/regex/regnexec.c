// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <regex.h>

int regnexec(const regex_t *restrict preg, const char *restrict string,
             size_t len, size_t nmatch, regmatch_t *restrict pmatch,
             int eflags) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return regnexec_l(preg, string, len, nmatch, pmatch, eflags, locale);
}
