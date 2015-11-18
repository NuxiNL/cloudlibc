// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <dirent.h>
#include <testing.h>

TEST(dirfd, example) {
  DIR *dirp = fdopendir(fd_tmp);
  ASSERT_EQ(fd_tmp, dirfd(dirp));
  ASSERT_EQ(fd_tmp, fdclosedir(dirp));
}
