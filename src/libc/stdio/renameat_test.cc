// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(renameat, bad) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  ASSERT_EQ(-1, renameat(fd_tmp, "foo", fd_tmp, "bar"));
  ASSERT_EQ(ENOENT, errno);

  ASSERT_EQ(-1, renameat(fd_tmp, ".", fd_tmp, "bar"));
  ASSERT_EQ(EINVAL, errno);
}

TEST(renameat, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Open two directories for testing.
  ASSERT_EQ(0, mkdirat(fd_tmp, "dir1"));
  int fd1 = openat(fd_tmp, "dir1", O_RDONLY | O_DIRECTORY);
  ASSERT_LE(0, fd1);

  ASSERT_EQ(0, mkdirat(fd_tmp, "dir2"));
  int fd2 = openat(fd_tmp, "dir2", O_RDONLY | O_DIRECTORY);
  ASSERT_LE(0, fd2);

  // Create a regular file in directory 1 and move it to directory 2.
  int fdreg = openat(fd1, "reg1", O_CREAT | O_WRONLY);
  ASSERT_LE(0, fdreg);
  ASSERT_EQ(0, close(fdreg));
  ASSERT_EQ(0, renameat(fd1, "reg1", fd2, "reg2"));

  // Validate that the regular file has moved.
  struct stat sb;
  ASSERT_EQ(-1, fstatat(fd1, "reg1", &sb, 0));
  ASSERT_EQ(ENOENT, errno);
  ASSERT_EQ(0, fstatat(fd2, "reg2", &sb, 0));
  ASSERT_TRUE(S_ISREG(sb.st_mode));

  ASSERT_EQ(0, close(fd1));
  ASSERT_EQ(0, close(fd2));
}
