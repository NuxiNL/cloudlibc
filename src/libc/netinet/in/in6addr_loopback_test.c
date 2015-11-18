// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <netinet/in.h>

#include <testing.h>

TEST(in6addr_loopback, macros) {
  ASSERT_FALSE(IN6_IS_ADDR_UNSPECIFIED(&in6addr_loopback));
  ASSERT_TRUE(IN6_IS_ADDR_LOOPBACK(&in6addr_loopback));
  ASSERT_FALSE(IN6_IS_ADDR_MULTICAST(&in6addr_loopback));
  ASSERT_FALSE(IN6_IS_ADDR_LINKLOCAL(&in6addr_loopback));
  ASSERT_FALSE(IN6_IS_ADDR_SITELOCAL(&in6addr_loopback));
  ASSERT_FALSE(IN6_IS_ADDR_V4MAPPED(&in6addr_loopback));
  ASSERT_FALSE(IN6_IS_ADDR_V4COMPAT(&in6addr_loopback));
  ASSERT_FALSE(IN6_IS_ADDR_MC_NODELOCAL(&in6addr_loopback));
  ASSERT_FALSE(IN6_IS_ADDR_MC_LINKLOCAL(&in6addr_loopback));
  ASSERT_FALSE(IN6_IS_ADDR_MC_SITELOCAL(&in6addr_loopback));
  ASSERT_FALSE(IN6_IS_ADDR_MC_ORGLOCAL(&in6addr_loopback));
  ASSERT_FALSE(IN6_IS_ADDR_MC_GLOBAL(&in6addr_loopback));
}
