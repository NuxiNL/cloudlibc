// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <dirent.h>
#include <testing.h>

TEST(dirfd, example) {
  DIR *dirp = fdopendir(fd_tmp);
  ASSERT_EQ(fd_tmp, dirfd(dirp));
  ASSERT_EQ(fd_tmp, fdclosedir(dirp));
}
