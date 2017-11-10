// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wctype.h>

#include "wctype_impl.h"

int iswctype_l(wint_t wc, wctype_t charclass, locale_t locale) {
  return iswctype(wc, charclass) && valid_in_locale(wc, locale);
}
