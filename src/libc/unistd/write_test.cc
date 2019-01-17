// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(write, bad) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Bad file descriptor.
  char b;
  ASSERT_EQ(-1, write(0xdeadc0de, &b, 1));
  ASSERT_EQ(EBADF, errno);

  // Invalid file descriptor type.
  ASSERT_EQ(-1, write(fd_tmp, &b, 1));
  ASSERT_EQ(EBADF, errno);
}

TEST(write, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Create pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));

  // Send string across.
  ASSERT_EQ(5, write(fds[1], "Hello", 5));
  char buf[6];
  ASSERT_EQ(5, read(fds[0], buf, sizeof(buf)));
  buf[5] = '\0';
  ASSERT_STREQ("Hello", buf);

  // Close pipe.
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}
