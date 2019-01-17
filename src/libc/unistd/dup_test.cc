// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(dup, file) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  int fd1 = openat(fd_tmp, "hello", O_CREAT | O_EXCL | O_WRONLY);
  ASSERT_LE(0, fd1);

  int fd2 = dup(fd1);
  ASSERT_LE(0, fd2);
  ASSERT_NE(fd1, fd2);

  // Both descriptors should point to the same file.
  struct stat sb1, sb2;
  ASSERT_EQ(0, fstat(fd1, &sb1));
  ASSERT_EQ(0, fstat(fd2, &sb2));
  ASSERT_EQ(sb1.st_dev, sb2.st_dev);
  ASSERT_EQ(sb1.st_ino, sb2.st_ino);

  ASSERT_EQ(0, close(fd1));
  ASSERT_EQ(0, close(fd2));
}

TEST(dup, bad) {
  ASSERT_EQ(-1, dup(-42));
  ASSERT_EQ(EBADF, errno);
}
