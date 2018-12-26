// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <common/locale.h>

#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <cloudlibc_interceptors.h>

int __cloudlibc_vsprintf(char *restrict s, const char *restrict format, va_list ap) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_NUMERIC_MASK);
  return vsnprintf_l(s, SIZE_MAX, locale, format, ap);
}

__weak_reference(__cloudlibc_vsprintf, vsprintf);
