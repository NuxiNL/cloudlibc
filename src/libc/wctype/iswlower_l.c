// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wctype.h>

#include "wctype_impl.h"

int iswlower_l(wint_t wc, locale_t locale) {
  return iswlower(wc) && valid_in_locale(wc, locale);
}
