// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>
#include <testing.h>

TEST(strcmp, examples) {
  ASSERT_EQ(0, strcmp("", ""));
  ASSERT_EQ(0, strcmp("Hello", "Hello"));

  ASSERT_GT(0, strcmp("Hello", "Hello, world"));
  ASSERT_LT(0, strcmp("Hello, world", "Hello"));

  ASSERT_GT(0, strcmp("Hello!", "Hello."));
  ASSERT_LT(0, strcmp("Hello.", "Hello!"));
}
