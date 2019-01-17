// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(ftruncate, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  int fd = openat(fd_tmp, "example", O_RDWR | O_CREAT | O_EXCL);
  ASSERT_LE(0, fd);

  // Initial length should be zero.
  struct stat sb;
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_EQ(0, sb.st_size);

  // Invalid length.
  ASSERT_EQ(-1, ftruncate(fd, -5));
  ASSERT_EQ(EINVAL, errno);

  // Extend file.
  ASSERT_EQ(0, ftruncate(fd, 100));
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_EQ(100, sb.st_size);

  // Shrink it again.
  ASSERT_EQ(0, ftruncate(fd, 50));
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_EQ(50, sb.st_size);

  ASSERT_EQ(0, close(fd));
}
