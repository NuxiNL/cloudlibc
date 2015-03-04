// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>
#include <testing.h>

TEST(strcspn, example) {
  const char *str = "Hello, world";
  ASSERT_EQ(0, strcspn(str, "H"));
  ASSERT_EQ(7, strcspn(str, "rdw"));
  ASSERT_EQ(12, strcspn(str, "XYZ"));
}
