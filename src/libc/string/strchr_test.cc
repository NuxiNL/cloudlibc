// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gtest/gtest.h"

TEST(strchr, examples) {
  const char *str = "Hello, world";
  ASSERT_EQ(NULL, strchr(str, 'A'));
  ASSERT_EQ(str + 4, strchr(str, 'o'));
  ASSERT_EQ(str + 12, strchr(str, '\0'));
}
