// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <string.h>
#include <testing.h>

TEST(strerror, success) {
  errno = 1234;
  ASSERT_STREQ("Success", strerror(0));
  ASSERT_EQ(1234, errno);
}

TEST(strerror, unknown) {
  errno = 1234;
  ASSERT_STREQ("Unknown error", strerror(-1));
  ASSERT_EQ(1234, errno);
}

TEST(strerror, einval) {
  errno = 1234;
  ASSERT_STREQ("Invalid argument", strerror(EINVAL));
  ASSERT_EQ(1234, errno);
}
