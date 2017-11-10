// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wctype.h>

int iswxdigit(wint_t wc) {
  return (wc >= 0x30 && wc <= 0x39) || (wc >= 0x41 && wc <= 0x46) ||
         (wc >= 0x61 && wc <= 0x66);
}
