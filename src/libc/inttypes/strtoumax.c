// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <inttypes.h>
#include <locale.h>

uintmax_t strtoumax(const char *restrict nptr, char **restrict endptr,
                    int base) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return strtoumax_l(nptr, endptr, base, locale);
}
