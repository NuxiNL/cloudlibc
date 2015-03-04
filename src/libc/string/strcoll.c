// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <string.h>

int strcoll(const char *s1, const char *s2) {
  DEFAULT_LOCALE(locale, LC_COLLATE_MASK | LC_CTYPE_MASK);
  return strcoll_l(s1, s2, locale);
}
