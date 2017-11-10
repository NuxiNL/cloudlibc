// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#ifndef abs
#error "abs is supposed to be a macro as well"
#endif

// clang-format off
int (abs)(int i) {
  return abs(i);
}
