// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>
#include <testing.h>

TEST(strstr, examples) {
  const char *str = "Hello world";
  ASSERT_EQ(str, strstr(str, ""));
  ASSERT_EQ(str + 2, strstr(str, "ll"));
  ASSERT_EQ(str + 4, strstr(str, "o worl"));
  ASSERT_EQ(str + 6, strstr(str, "world"));
  ASSERT_EQ(str + 10, strstr(str, "d"));
  ASSERT_EQ(NULL, strstr(str, "word"));
  ASSERT_EQ(NULL, strstr(str, "world!"));
}
