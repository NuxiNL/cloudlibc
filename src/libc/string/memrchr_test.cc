// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gtest/gtest.h"

TEST(memrchr, null) {
  ASSERT_EQ(NULL, memrchr(NULL, 'A', 0));
}

TEST(memrchr, match) {
  char buf[] = "Foo bar baz";
  ASSERT_EQ(buf + 9, memrchr(buf, 'a', sizeof(buf)));
}

TEST(memrchr, nomatch) {
  char buf[] = "Foo bar baz";
  ASSERT_EQ(NULL, memrchr(buf, 'x', sizeof(buf)));
}
