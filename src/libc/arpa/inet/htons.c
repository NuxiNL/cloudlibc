// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <arpa/inet.h>

#ifndef htons
#error "htons is supposed to be a macro as well"
#endif

// clang-format off
uint16_t (htons)(uint16_t i) {
  return htons(i);
}
