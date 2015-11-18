// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <strings.h>

int strcasecmp(const char *s1, const char *s2) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return strcasecmp_l(s1, s2, locale);
}
