// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <fnmatch.h>
#include <locale.h>
#include <regex.h>

static_assert(FNM_PATHNAME == REG_FNMATCH_PATHNAME, "Value mismatch");
static_assert(FNM_PERIOD == REG_FNMATCH_PERIOD, "Value mismatch");
static_assert(FNM_NOESCAPE == REG_FNMATCH_NOESCAPE, "Value mismatch");

int fnmatch_l(const char *pattern, const char *string, int flags,
              locale_t locale) {
  regex_t reg;
  if (regcomp_l(&reg, pattern, REG_NOSUB | REG_FNMATCH | flags, locale) != 0)
    return FNM_NOMATCH;
  int result = regexec_l(&reg, string, 0, NULL, 0, locale);
  regfree(&reg);
  return result == 0 ? 0 : FNM_NOMATCH;
}
