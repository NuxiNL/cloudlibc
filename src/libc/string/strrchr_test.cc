// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gtest/gtest.h"

TEST(strrchr, examples) {
  const char *str = "Hello, world";
  ASSERT_EQ(NULL, strrchr(str, 'A'));
  ASSERT_EQ(str + 8, strrchr(str, 'o'));
  ASSERT_EQ(str + 12, strrchr(str, '\0'));
}
