// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>
#include <testing.h>

TEST(strspn, example) {
  const char *str = "Hello, world";
  ASSERT_EQ(0, strspn(str, ""));
  ASSERT_EQ(0, strspn(str, "Foo"));
  ASSERT_EQ(5, strspn(str, "olHe"));
  ASSERT_EQ(12, strspn(str, "Helo, wrld"));
}
