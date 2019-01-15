// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <string.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(strerror, null) {
  errno = 1234;
  ASSERT_EQ(0, strerror_r(EINVAL, NULL, 0));
  ASSERT_EQ(1234, errno);
}

TEST(strerror, one) {
  char buf = 'A';
  errno = 1234;
  ASSERT_EQ(0, strerror_r(EINVAL, &buf, 1));
  ASSERT_EQ(1234, errno);
  ASSERT_EQ('\0', buf);
}

TEST(strerror, partial) {
  char buf[12];
  errno = 1234;
  ASSERT_EQ(0, strerror_r(EINVAL, buf, sizeof(buf)));
  ASSERT_EQ(1234, errno);
  ASSERT_STREQ("Invalid arg", buf);
}

TEST(strerror, full) {
  char buf[21] = "AAAAAAAAAAAAAAAAAAAA";
  errno = 1234;
  ASSERT_EQ(0, strerror_r(EINVAL, buf, sizeof(buf)));
  ASSERT_EQ(1234, errno);
  ASSERT_THAT(buf, testing::ElementsAreArray("Invalid argument\x00"
                                             "AAA"));
}
