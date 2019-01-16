// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <dirent.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(dirfd, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();
  DIR *dirp = fdopendir(fd_tmp);
  ASSERT_EQ(fd_tmp, dirfd(dirp));
  ASSERT_EQ(fd_tmp, fdclosedir(dirp));
}
