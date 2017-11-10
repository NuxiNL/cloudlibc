// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdio.h>

FILE *tmpfile(void) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return tmpfile_l(locale);
}
