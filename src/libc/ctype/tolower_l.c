// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <ctype.h>
#include <wctype.h>

#include "ctype_impl.h"

int tolower_l(int c, locale_t locale) {
  return toctrans_l(c, towlower, locale);
}
