// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <locale.h>

struct lconv *localeconv(void) {
  return localeconv_l(LC_GLOBAL_LOCALE);
}
