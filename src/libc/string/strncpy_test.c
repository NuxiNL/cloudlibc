// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>
#include <testing.h>

TEST(strncpy, null) {
  ASSERT_EQ((char *)12, strncpy((char *)12, (char *)500, 0));
}

TEST(strncpy, example1) {
  char buf[12] = "AAAAAAAAAAAA";
  ASSERT_EQ(buf, strncpy(buf, "Hello", 12));
  ASSERT_ARREQ("Hello\0\0\0\0\0\0\0", buf, 12);
}

TEST(strncpy, example2) {
  char buf[12];
  ASSERT_EQ(buf, strncpy(buf, "This is a very long string", 12));
  ASSERT_ARREQ("This is a ve", buf, 12);
}
