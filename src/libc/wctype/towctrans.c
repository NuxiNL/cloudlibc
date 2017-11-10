// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stddef.h>
#include <wctype.h>

wint_t towctrans(wint_t wc, wctrans_t desc) {
  return desc != NULL ? desc(wc) : wc;
}
