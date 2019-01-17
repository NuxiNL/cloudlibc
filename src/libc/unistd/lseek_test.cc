// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(lseek, bad) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  ASSERT_EQ(-1, lseek(-1, 0, SEEK_SET));
  ASSERT_EQ(EBADF, errno);

  ASSERT_EQ(-1, lseek(fd_tmp, 0, 0xdeadc0de));
  ASSERT_EQ(EINVAL, errno);

  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(-1, lseek(fds[0], 10, SEEK_CUR));
  ASSERT_EQ(ESPIPE, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

TEST(lseek, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Create example file.
  int fd = openat(fd_tmp, "file", O_CREAT | O_RDWR);
  ASSERT_LE(0, fd);
  ASSERT_EQ(0, ftruncate(fd, 1000));

  // Test SEEK_SET.
  ASSERT_EQ(-1, lseek(fd, -10, SEEK_SET));
  ASSERT_EQ(EINVAL, errno);
  ASSERT_EQ(0, lseek(fd, 0, SEEK_SET));
  ASSERT_EQ(10000, lseek(fd, 10000, SEEK_SET));
  ASSERT_EQ(10, lseek(fd, 10, SEEK_SET));

  // Test SEEK_CUR.
  ASSERT_EQ(9, lseek(fd, -1, SEEK_CUR));
  ASSERT_EQ(7, lseek(fd, -2, SEEK_CUR));
  ASSERT_EQ(4, lseek(fd, -3, SEEK_CUR));
  ASSERT_EQ(4, lseek(fd, 0, SEEK_CUR));
  ASSERT_EQ(14, lseek(fd, 10, SEEK_CUR));

  // Test SEEK_END.
  ASSERT_EQ(990, lseek(fd, -10, SEEK_END));
  ASSERT_EQ(1000, lseek(fd, 0, SEEK_END));
  ASSERT_EQ(1010, lseek(fd, 10, SEEK_END));
  ASSERT_EQ(0, close(fd));
}
