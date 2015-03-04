// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <locale.h>
#include <regex.h>
#include <string.h>

int regexec_l(const regex_t *restrict preg, const char *restrict string,
              size_t nmatch, regmatch_t *restrict pmatch, int eflags,
              locale_t locale) {
  return regnexec_l(preg, string, strlen(string), nmatch, pmatch, eflags,
                    locale);
}
