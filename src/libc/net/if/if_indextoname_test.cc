// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <net/if.h>

#include <stddef.h>

#include "gtest/gtest.h"

TEST(if_indextoname, examples) {
  // Interface 0 does not exist.
  ASSERT_EQ(NULL, if_indextoname(0, NULL));

  // Other interfaces do exist.
  char ifname[IF_NAMESIZE];
  ASSERT_EQ(ifname, if_indextoname(1, ifname));
  ASSERT_STREQ("1", ifname);
  ASSERT_EQ(ifname, if_indextoname(123, ifname));
  ASSERT_STREQ("123", ifname);
  ASSERT_EQ(ifname, if_indextoname(4294967295, ifname));
  ASSERT_STREQ("4294967295", ifname);
}
