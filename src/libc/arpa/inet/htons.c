// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <arpa/inet.h>

#ifndef htons
#error "htons is supposed to be a macro as well"
#endif

// clang-format off
uint16_t (htons)(uint16_t i) {
  return htons(i);
}
