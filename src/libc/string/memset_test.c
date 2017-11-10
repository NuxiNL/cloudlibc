// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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
