// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>
#include <testing.h>

TEST(strlen, all) {
  ASSERT_EQ(0, strlen(""));
  ASSERT_EQ(12, strlen("Hello, world"));
}
