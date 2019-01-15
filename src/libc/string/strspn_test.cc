// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gtest/gtest.h"

TEST(strspn, example) {
  const char *str = "Hello, world";
  ASSERT_EQ(0, strspn(str, ""));
  ASSERT_EQ(0, strspn(str, "Foo"));
  ASSERT_EQ(5, strspn(str, "olHe"));
  ASSERT_EQ(12, strspn(str, "Helo, wrld"));
}
