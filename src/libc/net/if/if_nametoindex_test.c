// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <net/if.h>

#include <testing.h>

TEST(if_nametoindex, examples) {
  // Bad interface names.
  ASSERT_EQ(0, if_nametoindex(""));
  ASSERT_EQ(0, if_nametoindex("0"));
  ASSERT_EQ(0, if_nametoindex("000123"));
  ASSERT_EQ(0, if_nametoindex("foo123"));
  ASSERT_EQ(0, if_nametoindex("123foo"));

  // Correct interface names.
  ASSERT_EQ(1, if_nametoindex("1"));
  ASSERT_EQ(123, if_nametoindex("123"));
  ASSERT_EQ(4294967295, if_nametoindex("4294967295"));
}
