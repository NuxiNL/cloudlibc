// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>

#ifndef labs
#error "labs is supposed to be a macro as well"
#endif

// clang-format off
long long (llabs)(long long i) {
  return llabs(i);
}
