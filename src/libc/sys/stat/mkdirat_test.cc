// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/stat.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(mkdirat, simple) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  ASSERT_EQ(0, mkdirat(fd_tmp, "example"));

  struct stat sb;
  ASSERT_EQ(0, fstatat(fd_tmp, "example", &sb, 0));
  ASSERT_TRUE(S_ISDIR(sb.st_mode));
  // Though most UNIX file systems use a link count of 2 for empty
  // directories, btrfs does not. It always uses a link count of 1.
  ASSERT_LE(1, sb.st_nlink);
  ASSERT_GE(2, sb.st_nlink);
}
