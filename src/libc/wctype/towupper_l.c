// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wctype.h>

#include "wctype_impl.h"

wint_t towupper_l(wint_t wc, locale_t locale) {
  wint_t nwc = towupper(wc);
  return valid_in_locale(nwc, locale) ? nwc : wc;
}
