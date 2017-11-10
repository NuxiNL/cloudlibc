// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <strings.h>

#ifndef ffsll
#error "ffsll is supposed to be a macro as well"
#endif

// clang-format off
int (ffsll)(long long i) {
  return ffsll(i);
}
