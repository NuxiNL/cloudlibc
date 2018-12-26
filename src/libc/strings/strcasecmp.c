// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <strings.h>
#include <cloudlibc_interceptors.h>

int __cloudlibc_strcasecmp(const char *s1, const char *s2) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return strcasecmp_l(s1, s2, locale);
}

__weak_reference(__cloudlibc_strcasecmp, strcasecmp);
