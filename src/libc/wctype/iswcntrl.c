// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wctype.h>

int iswcntrl(wint_t wc) {
  return (wc >= 0x0 && wc <= 0x1f) || (wc >= 0x7f && wc <= 0x9f) ||
         (wc >= 0x2028 && wc <= 0x2029);
}
