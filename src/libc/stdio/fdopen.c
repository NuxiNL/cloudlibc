// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdio.h>
#include <cloudlibc_interceptors.h>

FILE *__cloudlibc_fdopen(int fildes, const char *mode) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return fdopen_l(fildes, mode, locale);
}

__weak_reference(__cloudlibc_fdopen, fdopen);
