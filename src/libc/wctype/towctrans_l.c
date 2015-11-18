// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wctype.h>

#include "wctype_impl.h"

wint_t towctrans_l(wint_t wc, wctrans_t desc, locale_t locale) {
  wint_t nwc = towctrans(wc, desc);
  return valid_in_locale(nwc, locale) ? nwc : wc;
}
