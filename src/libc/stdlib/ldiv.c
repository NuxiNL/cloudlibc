// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#ifndef ldiv
#error "ldiv is supposed to be a macro as well"
#endif

// clang-format off
ldiv_t (ldiv)(long numer, long denom) {
  return ldiv(numer, denom);
}
