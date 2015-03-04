// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>
#include <testing.h>

TEST(memset, null) {
  ASSERT_EQ((char *)5, memset((char *)5, 'A', 0));
}

TEST(memset, example_small) {
  char buf[10];
  ASSERT_EQ(buf, memset(buf, '!', sizeof(buf)));
  ASSERT_ARREQ("!!!!!!!!!!", buf, __arraycount(buf));
}

TEST(memset, example_large) {
  char buf[100];
  ASSERT_EQ(buf, memset(buf, '!', __arraycount(buf)));
  ASSERT_ARREQ(
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
      buf, sizeof(buf));
}
