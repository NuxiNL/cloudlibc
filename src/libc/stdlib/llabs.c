// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#ifndef labs
#error "labs is supposed to be a macro as well"
#endif

// clang-format off
long long (llabs)(long long i) {
  return llabs(i);
}
