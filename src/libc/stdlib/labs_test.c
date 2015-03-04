// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <limits.h>
#include <stdlib.h>
#include <testing.h>

TEST(labs, example) {
  ASSERT_EQ(0, labs(0));
  ASSERT_EQ(42, labs(42));
  ASSERT_EQ(42, labs(-42));
  ASSERT_EQ(LONG_MAX, labs(LONG_MAX));
  ASSERT_EQ(LONG_MAX, labs(-LONG_MAX));
}
