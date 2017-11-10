// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <strings.h>

#ifndef ffs
#error "ffs is supposed to be a macro as well"
#endif

// clang-format off
int (ffs)(int i) {
  return ffs(i);
}
