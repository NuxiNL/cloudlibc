// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <arpa/inet.h>

#ifndef ntohl
#error "ntohl is supposed to be a macro as well"
#endif

// clang-format off
uint32_t (ntohl)(uint32_t i) {
  return ntohl(i);
}
