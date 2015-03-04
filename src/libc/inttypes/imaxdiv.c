// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <inttypes.h>

#ifndef imaxdiv
#error "imaxdiv is supposed to be a macro as well"
#endif

// clang-format off
imaxdiv_t (imaxdiv)(intmax_t numer, intmax_t denom) {
  return imaxdiv(numer, denom);
}
