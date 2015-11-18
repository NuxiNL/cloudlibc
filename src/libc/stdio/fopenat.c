// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <stdio.h>

FILE *fopenat(int fd, const char *restrict pathname,
              const char *restrict mode) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return fopenat_l(fd, pathname, mode, locale);
}
