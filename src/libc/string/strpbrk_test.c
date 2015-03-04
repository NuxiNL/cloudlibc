// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>
#include <testing.h>

TEST(strpbrk, example) {
  const char *str = "Hello, world";
  ASSERT_EQ(str, strpbrk(str, "H"));
  ASSERT_EQ(str + 7, strpbrk(str, "rdw"));
  ASSERT_EQ(NULL, strpbrk(str, "XYZ"));
}
