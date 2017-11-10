// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <arpa/inet.h>

#ifndef htonl
#error "htonl is supposed to be a macro as well"
#endif

// clang-format off
uint32_t (htonl)(uint32_t i) {
  return htonl(i);
}
