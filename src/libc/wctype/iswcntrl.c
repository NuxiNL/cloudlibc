// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <wctype.h>

int iswcntrl(wint_t wc) {
  return (
#if WINT_MIN < 0
             wc >= 0x0 &&
#endif
             wc <= 0x1f) ||
         (wc >= 0x7f && wc <= 0x9f) || (wc >= 0x2028 && wc <= 0x2029);
}
