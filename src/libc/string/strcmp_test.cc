// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gtest/gtest.h"

TEST(strcmp, examples) {
  ASSERT_EQ(0, strcmp("", ""));
  ASSERT_EQ(0, strcmp("Hello", "Hello"));

  ASSERT_GT(0, strcmp("Hello", "Hello, world"));
  ASSERT_LT(0, strcmp("Hello, world", "Hello"));

  ASSERT_GT(0, strcmp("Hello!", "Hello."));
  ASSERT_LT(0, strcmp("Hello.", "Hello!"));
}
