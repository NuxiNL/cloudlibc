// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wctype.h>

int iswblank(wint_t wc) {
  return wc == 0x9 || wc == 0x20 || wc == 0x1680 ||
         (wc >= 0x2000 && wc <= 0x2006) || (wc >= 0x2008 && wc <= 0x200a) ||
         wc == 0x205f || wc == 0x3000;
}
