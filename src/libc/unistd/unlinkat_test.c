// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
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

TEST(unlinkat, symlinks_directory) {
  // A symlink to a directory is not a directory.
  ASSERT_EQ(0, mkdirat(fd_tmp, "dir"));
  ASSERT_EQ(0, symlinkat("dir", fd_tmp, "symlink"));
  ASSERT_EQ(-1, unlinkat(fd_tmp, "symlink", AT_REMOVEDIR));
  ASSERT_EQ(ENOTDIR, errno);

  // Trailing slash should cause us to follow the directory and remove
  // that instead.
  ASSERT_EQ(-1, unlinkat(fd_tmp, "symlink/", 0));
  ASSERT_EQ(EPERM, errno);
  ASSERT_EQ(0, unlinkat(fd_tmp, "symlink/", AT_REMOVEDIR));
  ASSERT_EQ(-1, unlinkat(fd_tmp, "dir", 0));
  ASSERT_EQ(ENOENT, errno);
}

TEST(unlinkat, symlinks_fifo) {
  // The trailing slash should only be usable on symlinks to
  // directories. On symlinks to non-directories, we should always
  // return ENOTDIR.
  ASSERT_EQ(0, mkfifoat(fd_tmp, "fifo"));
  ASSERT_EQ(0, symlinkat("fifo", fd_tmp, "symlink"));
  ASSERT_EQ(-1, unlinkat(fd_tmp, "symlink/", AT_REMOVEDIR));
  ASSERT_EQ(ENOTDIR, errno);
  ASSERT_EQ(-1, unlinkat(fd_tmp, "symlink/", 0));
  ASSERT_EQ(ENOTDIR, errno);
}
