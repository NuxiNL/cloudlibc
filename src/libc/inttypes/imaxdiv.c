// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <inttypes.h>

#ifndef imaxdiv
#error "imaxdiv is supposed to be a macro as well"
#endif

// clang-format off
imaxdiv_t (imaxdiv)(intmax_t numer, intmax_t denom) {
  return imaxdiv(numer, denom);
}
