// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdlib.h>

long strtol(const char *restrict str, char **restrict endptr, int base) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return strtol_l(str, endptr, base, locale);
}
