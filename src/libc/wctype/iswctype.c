// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stddef.h>
#include <wctype.h>

int iswctype(wint_t wc, wctype_t charclass) {
  return charclass != NULL && charclass(wc);
}
