// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <regex.h>
#include <string.h>

int regexec(const regex_t *restrict preg, const char *restrict string,
            size_t nmatch, regmatch_t *restrict pmatch, int eflags) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return regnexec_l(preg, string, strlen(string), nmatch, pmatch, eflags,
                    locale);
}
