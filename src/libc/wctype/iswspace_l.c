// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wctype.h>

#include "wctype_impl.h"

int iswspace_l(wint_t wc, locale_t locale) {
  return iswspace(wc) && valid_in_locale(wc, locale);
}
