// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdio.h>
#include <cloudlibc_interceptors.h>

int __cloudlibc_vfscanf(FILE *restrict stream, const char *restrict format, va_list arg) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_NUMERIC_MASK);
  return vfscanf_l(stream, locale, format, arg);
}

__weak_reference(__cloudlibc_vfscanf, vfscanf);
