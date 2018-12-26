// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <strings.h>
#include <cloudlibc_interceptors.h>

int __cloudlibc_strncasecmp(const char *s1, const char *s2, size_t n) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return strncasecmp_l(s1, s2, n, locale);
}

__weak_reference(__cloudlibc_strncasecmp, strncasecmp);
