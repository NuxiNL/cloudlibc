// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <inttypes.h>
#include <locale.h>

intmax_t strtoimax(const char *restrict nptr, char **restrict endptr,
                   int base) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return strtoimax_l(nptr, endptr, base, locale);
}
