// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include "ctype_impl.h"

#include <ctype.h>
#include <wctype.h>

int toupper_l(int c, locale_t locale) {
  return toctrans_l(c, towupper, locale);
}
