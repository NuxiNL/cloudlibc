// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#ifndef lldiv
#error "lldiv is supposed to be a macro as well"
#endif

// clang-format off
lldiv_t (lldiv)(long long numer, long long denom) {
  return lldiv(numer, denom);
}
