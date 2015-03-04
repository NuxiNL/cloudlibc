// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <locale.h>
#include <stdio.h>

void perror(const char *s) {
  perror_l(s, LC_GLOBAL_LOCALE);
}
