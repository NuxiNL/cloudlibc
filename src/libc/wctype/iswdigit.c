// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wctype.h>

int iswdigit(wint_t wc) {
  return wc >= 0x30 && wc <= 0x39;
}
