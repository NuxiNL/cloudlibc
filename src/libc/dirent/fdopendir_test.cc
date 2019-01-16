// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/stat.h>

#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(fdopendir, bad) {
  // Invalid file descriptor.
  ASSERT_EQ(NULL, fdopendir(-123));
  ASSERT_EQ(EBADF, errno);

  // Not a directory.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(NULL, fdopendir(fds[0]));
  ASSERT_EQ(ENOTDIR, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

static void test_dot(int fd, const struct dirent *de) {
  struct stat sb;
  ASSERT_EQ(0, fstat(fd, &sb));

  ASSERT_EQ(sb.st_ino, de->d_ino);
  ASSERT_EQ(DT_DIR, de->d_type);
  ASSERT_STREQ(".", de->d_name);
}

static void test_dotdot(const struct dirent *de) {
  ASSERT_EQ(DT_DIR, de->d_type);
  ASSERT_STREQ("..", de->d_name);
}

TEST(fdopendir, empty) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  DIR *dirp = fdopendir(fd_tmp);
  ASSERT_NE(NULL, dirp);

  // We should see exactly two directory entries. The order may differ.
  struct dirent *de = readdir(dirp);
  if (strcmp(".", de->d_name) == 0) {
    test_dot(fd_tmp, de);
    de = readdir(dirp);
    test_dotdot(de);
  } else {
    test_dotdot(de);
    de = readdir(dirp);
    test_dot(fd_tmp, de);
  }

  ASSERT_EQ(NULL, readdir(dirp));
  ASSERT_EQ(fd_tmp, fdclosedir(dirp));
}
