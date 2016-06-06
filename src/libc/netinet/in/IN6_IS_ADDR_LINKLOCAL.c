// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <netinet/in.h>

#ifndef IN6_IS_ADDR_LINKLOCAL
#error "IN6_IS_ADDR_LINKLOCAL is supposed to be a macro as well"
#endif

// clang-format off
int (IN6_IS_ADDR_LINKLOCAL)(const struct in6_addr *a) {
  return IN6_IS_ADDR_LINKLOCAL(a);
}
