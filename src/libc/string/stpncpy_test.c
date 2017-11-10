// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>
#include <testing.h>

TEST(stpncpy, null) {
  ASSERT_EQ((char *)12, stpncpy((char *)12, (char *)500, 0));
}

TEST(stpncpy, example1) {
  char buf[12] = "AAAAAAAAAAAA";
  ASSERT_EQ(buf + 5, stpncpy(buf, "Hello", 12));
  ASSERT_ARREQ("Hello\0\0\0\0\0\0\0", buf, 12);
}

TEST(stpncpy, example2) {
  char buf[12];
  ASSERT_EQ(buf + 12, stpncpy(buf, "This is a very long string", 12));
  ASSERT_ARREQ("This is a ve", buf, 12);
}
