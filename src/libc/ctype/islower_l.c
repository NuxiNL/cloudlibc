// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <ctype.h>
#include <wctype.h>

#include "ctype_impl.h"

int islower_l(int c, locale_t locale) {
  return isctype_l(c, iswlower, locale);
}
