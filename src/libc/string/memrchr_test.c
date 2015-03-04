// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>
#include <testing.h>

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
