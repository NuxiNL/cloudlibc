// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wctype.h>

int iswdigit(wint_t wc) {
  return wc >= 0x30 && wc <= 0x39;
}
