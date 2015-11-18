// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <limits.h>
#include <strings.h>
#include <testing.h>

TEST(ffs, examples) {
  ASSERT_EQ(0, ffs(0x0));
  ASSERT_EQ(1, ffs(0x3211));
  ASSERT_EQ(2, ffs(0xabc2));
  ASSERT_EQ(3, ffs(0x79224));

  ASSERT_EQ(WORD_BIT - 3, ffs(INT_MIN >> 3));
  ASSERT_EQ(WORD_BIT, ffs(INT_MIN));
}
