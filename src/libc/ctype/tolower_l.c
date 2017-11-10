// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <ctype.h>
#include <wctype.h>

#include "ctype_impl.h"

int tolower_l(int c, locale_t locale) {
  return toctrans_l(c, towlower, locale);
}
