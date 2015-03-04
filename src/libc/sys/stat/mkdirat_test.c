// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/stat.h>

#include <testing.h>

TEST(mkdirat, simple) {
  ASSERT_EQ(0, mkdirat(fd_tmp, "example"));

  struct stat sb;
  ASSERT_EQ(0, fstatat(fd_tmp, "example", &sb, 0));
  ASSERT_TRUE(S_ISDIR(sb.st_mode));
  ASSERT_EQ(2, sb.st_nlink);
}
