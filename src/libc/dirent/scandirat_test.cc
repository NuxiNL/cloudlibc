// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(scandirat, bad) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Bad directory file descriptor.
  ASSERT_EQ(-1, scandirat(-123, "hello", nullptr, nullptr, nullptr));
  ASSERT_EQ(EBADF, errno);

  // Attempts to escape the directory.
  ASSERT_EQ(-1, scandirat(fd_tmp, "/foo", nullptr, nullptr, nullptr));
  ASSERT_EQ(ENOTCAPABLE, errno);
  ASSERT_EQ(-1, scandirat(fd_tmp, "../a", nullptr, nullptr, nullptr));
  ASSERT_EQ(ENOTCAPABLE, errno);

  // Empty pathname string.
  ASSERT_EQ(-1, scandirat(fd_tmp, "", nullptr, nullptr, nullptr));
  ASSERT_EQ(ENOENT, errno);

  // Calling scandirat() on a non-directory file descriptor.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(-1, scandirat(fds[0], "hello", nullptr, nullptr, nullptr));
  ASSERT_EQ(ENOTDIR, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

TEST(scandirat, empty) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Fetch directory listing of the temporary directory of the test.
  struct dirent **namelist;
  ASSERT_EQ(2, scandirat(fd_tmp, ".", &namelist, NULL, alphasort));

  // Validate results.
  ASSERT_EQ(DT_DIR, namelist[0]->d_type);
  ASSERT_STREQ(".", namelist[0]->d_name);
  ASSERT_EQ(DT_DIR, namelist[1]->d_type);
  ASSERT_STREQ("..", namelist[1]->d_name);

  // Free entries.
  free(namelist[0]);
  free(namelist[1]);
  free(namelist);
}

// Selects only evenly numbered entries from the directory.
static int sel_even(const struct dirent *de) {
  return strlen(de->d_name) != 7 || (de->d_name[6] % 2) == 0;
}

TEST(scandirat, even_files) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Create some example files.
  for (int i = 0; i < 1000; ++i) {
    char filename[8];
    snprintf(filename, sizeof(filename), "file%03d", i);
    ASSERT_EQ(0, close(openat(fd_tmp, filename, O_RDWR | O_CREAT)));
  }

  // Fetch only evenly numbered entries from the directory.
  struct dirent **namelist;
  ASSERT_EQ(502, scandirat(fd_tmp, ".", &namelist, sel_even, versionsort));

  // Validate results.
  ASSERT_EQ(DT_DIR, namelist[0]->d_type);
  ASSERT_STREQ(".", namelist[0]->d_name);
  ASSERT_EQ(DT_DIR, namelist[1]->d_type);
  ASSERT_STREQ("..", namelist[1]->d_name);
  for (int i = 0; i < 500; ++i) {
    struct dirent *de = namelist[i + 2];
    ASSERT_EQ(DT_REG, de->d_type);
    char filename[8];
    snprintf(filename, sizeof(filename), "file%03d", i * 2);
    ASSERT_STREQ(filename, de->d_name);
  }

  // Free entries.
  for (int i = 0; i < 502; ++i)
    free(namelist[i]);
  free(namelist);
}
