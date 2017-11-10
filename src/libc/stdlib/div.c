// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#ifndef div
#error "div is supposed to be a macro as well"
#endif

// clang-format off
div_t (div)(int numer, int denom) {
  return div(numer, denom);
}
