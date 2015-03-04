// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <limits.h>
#include <strings.h>
#include <testing.h>

TEST(ffsl, examples) {
  ASSERT_EQ(0, ffsl(0x0));
  ASSERT_EQ(1, ffsl(0x3211));
  ASSERT_EQ(2, ffsl(0xabc2));
  ASSERT_EQ(3, ffsl(0x79224));

  ASSERT_EQ(LONG_BIT - 3, ffsl(LONG_MIN >> 3));
  ASSERT_EQ(LONG_BIT, ffsl(LONG_MIN));
}
