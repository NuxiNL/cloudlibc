// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>

#ifndef div
#error "div is supposed to be a macro as well"
#endif

// clang-format off
div_t (div)(int numer, int denom) {
  return div(numer, denom);
}
