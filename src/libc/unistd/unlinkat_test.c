// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

TEST(unlinkat, examples) {
  ASSERT_EQ(-1, unlinkat(fd_tmp, "nonexistent", 0));
  ASSERT_EQ(ENOENT, errno);
  ASSERT_EQ(-1, unlinkat(fd_tmp, "nonexistent", AT_REMOVEDIR));
  ASSERT_EQ(ENOENT, errno);

  ASSERT_EQ(0, mkfifoat(fd_tmp, "fifo"));
  ASSERT_EQ(-1, unlinkat(fd_tmp, "fifo", AT_REMOVEDIR));
  ASSERT_EQ(ENOTDIR, errno);
  ASSERT_EQ(0, unlinkat(fd_tmp, "fifo", 0));
  ASSERT_EQ(-1, unlinkat(fd_tmp, "fifo", 0));
  ASSERT_EQ(ENOENT, errno);

  ASSERT_EQ(0, mkdirat(fd_tmp, "dir"));
  ASSERT_EQ(-1, unlinkat(fd_tmp, "dir", 0));
  ASSERT_EQ(EPERM, errno);
  ASSERT_EQ(0, unlinkat(fd_tmp, "dir", AT_REMOVEDIR));
  ASSERT_EQ(-1, unlinkat(fd_tmp, "dir", 0));
  ASSERT_EQ(ENOENT, errno);
}
