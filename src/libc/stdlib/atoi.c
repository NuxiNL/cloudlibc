// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#ifndef atoi
#error "atoi is supposed to be a macro as well"
#endif

// clang-format off
int (atoi)(const char *str) {
  return atoi(str);
}
