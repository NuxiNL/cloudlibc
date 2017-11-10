// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdio.h>

FILE *fopenat(int fd, const char *restrict pathname,
              const char *restrict mode) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return fopenat_l(fd, pathname, mode, locale);
}
