// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <limits.h>
#include <stdlib.h>
#include <testing.h>

TEST(abs, example) {
  ASSERT_EQ(0, abs(0));
  ASSERT_EQ(42, abs(42));
  ASSERT_EQ(42, abs(-42));
  ASSERT_EQ(INT_MAX, abs(INT_MAX));
  ASSERT_EQ(INT_MAX, abs(-INT_MAX));
}
