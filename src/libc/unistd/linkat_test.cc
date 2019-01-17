// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(linkat, directory) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  ASSERT_EQ(0, mkdirat(fd_tmp, "subdir1"));
  ASSERT_EQ(-1, linkat(fd_tmp, "subdir1", fd_tmp, "subdir2", 0));
  ASSERT_EQ(EPERM, errno);
}

TEST(linkat, reg) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  int fd = openat(fd_tmp, "reg1", O_CREAT | O_WRONLY);
  ASSERT_LE(0, fd);
  ASSERT_EQ(0, close(fd));
  ASSERT_EQ(0, linkat(fd_tmp, "reg1", fd_tmp, "reg2", 0));

  struct stat sb1, sb2;
  ASSERT_EQ(0, fstatat(fd_tmp, "reg1", &sb1, 0));
  ASSERT_EQ(0, fstatat(fd_tmp, "reg2", &sb2, 0));
  ASSERT_EQ(sb1.st_dev, sb2.st_dev);
  ASSERT_EQ(sb1.st_ino, sb2.st_ino);
  ASSERT_EQ(2, sb1.st_nlink);
  ASSERT_EQ(2, sb2.st_nlink);
}

TEST(linkat, symlink) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Test the AT_SYMLINK_FOLLOW flag.
  ASSERT_EQ(0, symlinkat("nonexistent", fd_tmp, "symlink1"));
  ASSERT_EQ(-1,
            linkat(fd_tmp, "symlink1", fd_tmp, "symlink2", AT_SYMLINK_FOLLOW));
  ASSERT_EQ(ENOENT, errno);
  ASSERT_EQ(0, linkat(fd_tmp, "symlink1", fd_tmp, "symlink2", 0));
}
