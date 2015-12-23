// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>

#ifndef lldiv
#error "lldiv is supposed to be a macro as well"
#endif

// clang-format off
lldiv_t (lldiv)(long long numer, long long denom) {
  return lldiv(numer, denom);
}
