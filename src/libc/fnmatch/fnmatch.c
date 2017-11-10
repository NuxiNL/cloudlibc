// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <fnmatch.h>
#include <locale.h>

int fnmatch(const char *pattern, const char *string, int flags) {
  DEFAULT_LOCALE(locale, LC_COLLATE_MASK | LC_CTYPE_MASK);
  return fnmatch_l(pattern, string, flags, locale);
}
