// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/stat.h>

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(opendirat, bad) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  ASSERT_EQ(NULL, opendirat(-123, "example"));
  ASSERT_EQ(EBADF, errno);

  ASSERT_EQ(NULL, opendirat(fd_tmp, "nonexistent"));
  ASSERT_EQ(ENOENT, errno);

  int fd = openat(fd_tmp, "file", O_WRONLY | O_CREAT);
  ASSERT_NE(-1, fd);
  ASSERT_EQ(0, close(fd));

  ASSERT_EQ(NULL, opendirat(fd_tmp, "file"));
  ASSERT_EQ(ENOTDIR, errno);
}

TEST(opendirat, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  ASSERT_EQ(0, mkdirat(fd_tmp, "dir"));

  DIR *dirp = opendirat(fd_tmp, "dir");
  ASSERT_NE(NULL, dirp);

  ASSERT_EQ(0, closedir(dirp));
}
