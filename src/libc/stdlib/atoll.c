// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#ifndef atoll
#error "atoll is supposed to be a macro as well"
#endif

// clang-format off
long long (atoll)(const char *str) {
  return atoll(str);
}
