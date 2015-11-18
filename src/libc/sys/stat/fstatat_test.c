// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/stat.h>

#include <errno.h>
#include <testing.h>
#include <unistd.h>

TEST(fstatat, bad) {
  // Not a directory.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  struct stat sb;
  ASSERT_EQ(-1, fstatat(fds[0], "hello", &sb, 0));
  ASSERT_EQ(ENOTDIR, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

TEST(fstatat, escape) {
  // Statting the test directory is allowed.
  struct stat sb;
  ASSERT_EQ(0, fstatat(fd_tmp, ".", &sb, 0));

  // Escaping to the parent directory is not.
  ASSERT_EQ(-1, fstatat(fd_tmp, "..", &sb, 0));
  ASSERT_EQ(ENOTCAPABLE, errno);

  // There are no absolute paths.
  ASSERT_EQ(-1, fstatat(fd_tmp, "/dev/null", &sb, 0));
  ASSERT_EQ(ENOTCAPABLE, errno);
}
