// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <stdlib.h>

long strtol(const char *restrict str, char **restrict endptr, int base) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return strtol_l(str, endptr, base, locale);
}
