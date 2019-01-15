// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gtest/gtest.h"

TEST(strpbrk, example) {
  const char *str = "Hello, world";
  ASSERT_EQ(str, strpbrk(str, "H"));
  ASSERT_EQ(str + 7, strpbrk(str, "rdw"));
  ASSERT_EQ(NULL, strpbrk(str, "XYZ"));
}
