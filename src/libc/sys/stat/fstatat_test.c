// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/stat.h>

#include <errno.h>
#include <testing.h>

TEST(fstatat, escape) {
  // Statting the test directory is allowed.
  struct stat sb;
  ASSERT_EQ(0, fstatat(fd_tmp, ".", &sb, 0));

  // Escaping to the parent directory is not.
  ASSERT_EQ(-1, fstatat(fd_tmp, "..", &sb, 0));
  ASSERT_EQ(EPERM, errno);

  // There are no absolute paths.
  ASSERT_EQ(-1, fstatat(fd_tmp, "/dev/null", &sb, 0));
  ASSERT_EQ(EPERM, errno);
}
