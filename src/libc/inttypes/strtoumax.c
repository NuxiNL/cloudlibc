// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <inttypes.h>
#include <locale.h>
#include <cloudlibc_interceptors.h>

uintmax_t __cloudlibc_strtoumax(const char *restrict nptr, char **restrict endptr,
                    int base) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return strtoumax_l(nptr, endptr, base, locale);
}

__weak_reference(__cloudlibc_strtoumax, strtoumax);
