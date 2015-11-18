// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wctype.h>

wctrans_t wctrans_l(const char *charclass, locale_t locale) {
  return wctrans(charclass);
}
