// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(dup2, bad) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Source file descriptor is invalid.
  ASSERT_EQ(-1, dup2(-1, fd_tmp));
  ASSERT_EQ(EBADF, errno);

  // Target file descriptor is invalid.
  ASSERT_EQ(-1, dup2(fd_tmp, -1));
  ASSERT_EQ(EBADF, errno);

  // It is not possible to create new file descriptors using dup2(), for
  // the reason that file descriptor order does not matter in this
  // environment. It is also inherently thread-unsafe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, close(fds[1]));
  ASSERT_EQ(-1, dup2(fds[0], fds[1]));
  ASSERT_EQ(EBADF, errno);
  ASSERT_EQ(0, close(fds[0]));
}

TEST(dup2, good) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Create two files for testing.
  int fd1 = openat(fd_tmp, "file1", O_CREAT | O_EXCL | O_WRONLY);
  ASSERT_LE(0, fd1);
  int fd2 = openat(fd_tmp, "file2", O_CREAT | O_EXCL | O_WRONLY);
  ASSERT_LE(0, fd2);
  ASSERT_NE(fd1, fd2);

  // Both descriptors should point to a different file.
  struct stat sb1, sb2;
  ASSERT_EQ(0, fstat(fd1, &sb1));
  ASSERT_EQ(0, fstat(fd2, &sb2));
  ASSERT_EQ(sb1.st_dev, sb2.st_dev);
  ASSERT_NE(sb1.st_ino, sb2.st_ino);

  ASSERT_EQ(fd2, dup2(fd1, fd2));

  // Both descriptors should now point to the same file.
  ASSERT_EQ(0, fstat(fd1, &sb1));
  ASSERT_EQ(0, fstat(fd2, &sb2));
  ASSERT_EQ(sb1.st_dev, sb2.st_dev);
  ASSERT_EQ(sb1.st_ino, sb2.st_ino);

  ASSERT_EQ(0, close(fd1));
  ASSERT_EQ(0, close(fd2));
}
