// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <netinet/in.h>

#ifndef IN6_IS_ADDR_UNSPECIFIED
#error "IN6_IS_ADDR_UNSPECIFIED is supposed to be a macro as well"
#endif

// clang-format off
int (IN6_IS_ADDR_UNSPECIFIED)(const struct in6_addr *a) {
  return IN6_IS_ADDR_UNSPECIFIED(a);
}
