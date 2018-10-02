// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdlib.h>
#include <cloudlibc_interceptors.h>

long long __cloudlibc_strtoll(const char *restrict str, char **restrict endptr, int base) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return strtoll_l(str, endptr, base, locale);
}


__weak_reference(__cloudlibc_strtoll, strtoll);
