// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#ifndef atol
#error "atol is supposed to be a macro as well"
#endif

// clang-format off
long (atol)(const char *str) {
  return atol(str);
}
