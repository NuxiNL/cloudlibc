// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <arpa/inet.h>

#ifndef ntohl
#error "ntohl is supposed to be a macro as well"
#endif

// clang-format off
uint32_t (ntohl)(uint32_t i) {
  return ntohl(i);
}
