// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <stdio.h>

void perror(const char *s) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_MESSAGES_MASK);
  perror_l(s, locale);
}
