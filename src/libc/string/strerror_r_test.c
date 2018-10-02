// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <string.h>
#include <testing.h>

TEST(strerror_r, null) {
  errno = 1234;
  ASSERT_EQ(0, strerror_r(EINVAL, NULL, 0));
  ASSERT_EQ(1234, errno);
}

TEST(strerror_r, one) {
  char buf = 'A';
  errno = 1234;
  ASSERT_EQ(0, strerror_r(EINVAL, &buf, 1));
  ASSERT_EQ(1234, errno);
  ASSERT_EQ('\0', buf);
}

TEST(strerror_r, partial) {
  char buf[12] = "AAAAAAAAAAAA";
  errno = 1234;
  ASSERT_EQ(0, strerror_r(EINVAL, buf, sizeof(buf)));
  ASSERT_EQ(1234, errno);
  ASSERT_ARREQ("Invalid arg", buf, 12);
}

TEST(strerror_r, full) {
  char buf[20] = "AAAAAAAAAAAAAAAAAAAA";
  errno = 1234;
  ASSERT_EQ(0, strerror_r(EINVAL, buf, sizeof(buf)));
  ASSERT_EQ(1234, errno);
  ASSERT_ARREQ(
      "Invalid argument\x00"
      "AAA",
      buf, 20);
}
