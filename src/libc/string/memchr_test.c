// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>
#include <testing.h>

TEST(memchr, null) {
  ASSERT_EQ(NULL, memchr(NULL, 'A', 0));
}

TEST(memchr, match) {
  char buf[] = "Foo bar baz";
  ASSERT_EQ(buf + 5, memchr(buf, 'a', sizeof(buf)));
}

TEST(memchr, nomatch) {
  char buf[] = "Foo bar baz";
  ASSERT_EQ(NULL, memchr(buf, 'x', sizeof(buf)));
}
