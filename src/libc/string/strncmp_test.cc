// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gtest/gtest.h"

TEST(strncmp, null) {
  ASSERT_EQ(0, strncmp(NULL, NULL, 0));
}

TEST(strncmp, examples) {
  ASSERT_EQ(0, strncmp("", "", 100));
  ASSERT_EQ(0, strncmp("Hello", "Hello", 100));

  ASSERT_EQ(0, strncmp("Hello", "Hello, world", 5));
  ASSERT_GT(0, strncmp("Hello", "Hello, world", 6));
  ASSERT_LT(0, strncmp("Hello, world", "Hello", 100));

  ASSERT_EQ(0, strncmp("Hello!", "Hello.", 5));
  ASSERT_GT(0, strncmp("Hello!", "Hello.", 6));
  ASSERT_LT(0, strncmp("Hello.", "Hello!", 100));
}
