// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(readlinkat, bad) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  ASSERT_EQ(-1, readlinkat(fd_tmp, "foo", NULL, 0));
  ASSERT_EQ(ENOENT, errno);

  ASSERT_EQ(-1, readlinkat(fd_tmp, ".", NULL, 0));
  ASSERT_EQ(EINVAL, errno);
}

TEST(readlinkat, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  char buf[13];
  ASSERT_EQ(0, symlinkat("Hello, world", fd_tmp, "foo"));

  ASSERT_EQ(11, readlinkat(fd_tmp, "foo", buf, 11));
  buf[11] = '\0';
  ASSERT_STREQ("Hello, worl", buf);

  ASSERT_EQ(12, readlinkat(fd_tmp, "foo", buf, 12));
  buf[12] = '\0';
  ASSERT_STREQ("Hello, world", buf);

  ASSERT_EQ(12, readlinkat(fd_tmp, "foo", buf, 13));
  buf[12] = '\0';
  ASSERT_STREQ("Hello, world", buf);
}
