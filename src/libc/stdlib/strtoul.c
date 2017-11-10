// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdlib.h>

unsigned long strtoul(const char *restrict str, char **restrict endptr,
                      int base) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return strtoul_l(str, endptr, base, locale);
}
