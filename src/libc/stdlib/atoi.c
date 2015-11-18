// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>

#ifndef atoi
#error "atoi is supposed to be a macro as well"
#endif

// clang-format off
int (atoi)(const char *str) {
  return atoi(str);
}
