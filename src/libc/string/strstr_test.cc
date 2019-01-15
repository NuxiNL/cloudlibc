// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gtest/gtest.h"

TEST(strstr, examples) {
  // If s2 points to a string with zero length, the function shall return s1.
  const char *str = (const char *)0x42;
  ASSERT_EQ(str, strstr(str, ""));

  str = "Hello world";
  ASSERT_EQ(str + 2, strstr(str, "ll"));
  ASSERT_EQ(str + 4, strstr(str, "o worl"));
  ASSERT_EQ(str + 6, strstr(str, "world"));
  ASSERT_EQ(str + 10, strstr(str, "d"));
  ASSERT_EQ(NULL, strstr(str, "word"));
  ASSERT_EQ(NULL, strstr(str, "world!"));
}
