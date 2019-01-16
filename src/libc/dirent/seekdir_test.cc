// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(seekdir, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Create some example files.
  for (int i = 0; i < 100; ++i) {
    char filename[8];
    snprintf(filename, sizeof(filename), "file%03d", i);
    ASSERT_EQ(0, close(openat(fd_tmp, filename, O_RDWR | O_CREAT)));
  }

  // seekdir(telldir()) should restore to the right location.
  DIR *dirp = fdopendir(fd_tmp);
  size_t dirs = 0, files = 0;
  struct dirent *de;
  while ((de = readdir(dirp)) != NULL) {
    if (de->d_name[0] == '.') {
      ASSERT_EQ(DT_DIR, de->d_type);
      ++dirs;
    } else {
      ASSERT_EQ(DT_REG, de->d_type);
      ++files;
    }
    seekdir(dirp, telldir(dirp));
  }
  ASSERT_EQ(2, dirs);
  ASSERT_EQ(100, files);
}
