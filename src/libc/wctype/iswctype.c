// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stddef.h>
#include <wctype.h>

int iswctype(wint_t wc, wctype_t charclass) {
  return charclass != NULL ? charclass(wc) : 0;
}
