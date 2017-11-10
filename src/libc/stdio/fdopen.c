// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdio.h>

FILE *fdopen(int fildes, const char *mode) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return fdopen_l(fildes, mode, locale);
}
