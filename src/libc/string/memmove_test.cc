// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gtest/gtest.h"

TEST(memmove, null) {
  ASSERT_EQ((char *)42, memmove((char *)42, (char *)34, 0));
}

TEST(memmove, example1) {
  char buf[] = "abcdefghijkl";
  ASSERT_EQ(buf, memmove(buf, buf + 4, 8));
  ASSERT_STREQ("efghijklijkl", buf);
}

TEST(memmove, example2) {
  char buf[] = "abcdefghijkl";
  ASSERT_EQ(buf + 4, memmove(buf + 4, buf, 8));
  ASSERT_STREQ("abcdabcdefgh", buf);
}
