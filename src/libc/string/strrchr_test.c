// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>
#include <testing.h>

TEST(strrchr, examples) {
  const char *str = "Hello, world";
  ASSERT_EQ(NULL, strrchr(str, 'A'));
  ASSERT_EQ(str + 8, strrchr(str, 'o'));
  ASSERT_EQ(str + 12, strrchr(str, '\0'));
}
