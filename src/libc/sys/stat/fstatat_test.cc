// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/stat.h>

#include <errno.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(fstatat, bad) {
  // Not a directory.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  struct stat sb;
  ASSERT_EQ(-1, fstatat(fds[0], "hello", &sb, 0));
  ASSERT_EQ(ENOTDIR, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

TEST(fstatat, escape) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Statting the test directory is allowed.
  struct stat sb;
  ASSERT_EQ(0, fstatat(fd_tmp, ".", &sb, 0));

  // Escaping to the parent directory is not.
  ASSERT_EQ(-1, fstatat(fd_tmp, "..", &sb, 0));
  ASSERT_EQ(ENOTCAPABLE, errno);

  // There are no absolute paths.
  ASSERT_EQ(-1, fstatat(fd_tmp, "/dev/null", &sb, 0));
  ASSERT_EQ(ENOTCAPABLE, errno);
}
