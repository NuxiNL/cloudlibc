// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdio.h>
#include <cloudlibc_interceptors.h>

int __cloudlibc_vfprintf(FILE *restrict stream, const char *restrict format, va_list ap) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_NUMERIC_MASK);
  return vfprintf_l(stream, locale, format, ap);
}

__weak_reference(__cloudlibc_vfprintf, vfprintf);
