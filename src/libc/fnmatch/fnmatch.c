// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <fnmatch.h>
#include <locale.h>

int fnmatch(const char *pattern, const char *string, int flags) {
  DEFAULT_LOCALE(locale, LC_COLLATE_MASK | LC_CTYPE_MASK);
  return fnmatch_l(pattern, string, flags, locale);
}
