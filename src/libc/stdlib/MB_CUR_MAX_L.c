// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <stdlib.h>

size_t MB_CUR_MAX_L(locale_t locale) {
  return locale->ctype->c32tomb_max;
}
