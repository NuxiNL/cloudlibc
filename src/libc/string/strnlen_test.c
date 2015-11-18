// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>
#include <testing.h>

TEST(strnlen, null) {
  ASSERT_EQ(0, strnlen(NULL, 0));
  ASSERT_EQ(0, strnlen("", 100));
  ASSERT_EQ(7, strnlen("Hello, world", 7));
}
