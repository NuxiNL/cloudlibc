// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>
#include <testing.h>

TEST(memcpy, null) {
  ASSERT_EQ((char *)42, memcpy((char *)42, (char *)123, 0));
}

TEST(memcpy, example) {
  const char buf1[8] = "Foo\0Bar";
  char buf2[8];
  ASSERT_EQ(buf2, memcpy(buf2, buf1, sizeof(buf1)));
  ASSERT_ARREQ(buf1, buf2, __arraycount(buf1));
}
