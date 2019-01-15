// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gtest/gtest.h"

TEST(memchr, null) {
  ASSERT_EQ(NULL, memchr((char *)nullptr, 'A', 0));
}

TEST(memchr, match) {
  char buf[] = "Foo bar baz";
  ASSERT_EQ(buf + 5, memchr(buf, 'a', sizeof(buf)));
}

TEST(memchr, nomatch) {
  char buf[] = "Foo bar baz";
  ASSERT_EQ(NULL, memchr(buf, 'x', sizeof(buf)));
}
