// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdio.h>
#include <cloudlibc_interceptors.h>

int __cloudlibc_vasprintf(char **s, const char *format, va_list ap) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_NUMERIC_MASK);
  return vasprintf_l(s, locale, format, ap);
}

__weak_reference(__cloudlibc_vasprintf, vasprintf);
