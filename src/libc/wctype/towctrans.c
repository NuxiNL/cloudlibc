// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stddef.h>
#include <wctype.h>

wint_t towctrans(wint_t wc, wctrans_t desc) {
  return desc != NULL ? desc(wc) : wc;
}
