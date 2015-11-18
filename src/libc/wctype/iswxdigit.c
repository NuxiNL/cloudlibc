// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wctype.h>

int iswxdigit(wint_t wc) {
  return (wc >= 0x30 && wc <= 0x39) || (wc >= 0x41 && wc <= 0x46) ||
         (wc >= 0x61 && wc <= 0x66);
}
