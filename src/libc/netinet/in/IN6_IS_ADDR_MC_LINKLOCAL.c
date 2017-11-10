// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <netinet/in.h>

#ifndef IN6_IS_ADDR_MC_LINKLOCAL
#error "IN6_IS_ADDR_MC_LINKLOCAL is supposed to be a macro as well"
#endif

// clang-format off
int (IN6_IS_ADDR_MC_LINKLOCAL)(const struct in6_addr *a) {
  return IN6_IS_ADDR_MC_LINKLOCAL(a);
}
