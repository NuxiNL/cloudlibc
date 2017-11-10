// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wctype.h>

int iswspace(wint_t wc) {
  return (wc >= 0x9 && wc <= 0xd) || wc == 0x20 || wc == 0x1680 ||
         (wc >= 0x2000 && wc <= 0x2006) || (wc >= 0x2008 && wc <= 0x200a) ||
         (wc >= 0x2028 && wc <= 0x2029) || wc == 0x205f || wc == 0x3000;
}
