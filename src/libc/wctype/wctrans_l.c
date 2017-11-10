// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wctype.h>

wctrans_t wctrans_l(const char *charclass, locale_t locale) {
  return wctrans(charclass);
}
