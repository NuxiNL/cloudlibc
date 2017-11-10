// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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

  int fd = openat(fd_tmp, "reg", O_CREAT | O_WRONLY);
  ASSERT_LE(0, fd);
  ASSERT_EQ(0, close(fd));
  ASSERT_EQ(-1, unlinkat(fd_tmp, "reg", AT_REMOVEDIR));
  ASSERT_EQ(ENOTDIR, errno);
  ASSERT_EQ(0, unlinkat(fd_tmp, "reg", 0));
  ASSERT_EQ(-1, unlinkat(fd_tmp, "reg", 0));
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

TEST(unlinkat, symlinks_reg) {
  // The trailing slash should only be usable on symlinks to
  // directories. On symlinks to non-directories, we should always
  // return ENOTDIR.
  int fd = openat(fd_tmp, "reg", O_CREAT | O_WRONLY);
  ASSERT_LE(0, fd);
  ASSERT_EQ(0, close(fd));
  ASSERT_EQ(0, symlinkat("reg", fd_tmp, "symlink"));
  ASSERT_EQ(-1, unlinkat(fd_tmp, "symlink/", AT_REMOVEDIR));
  ASSERT_EQ(ENOTDIR, errno);
  ASSERT_EQ(-1, unlinkat(fd_tmp, "symlink/", 0));
  ASSERT_EQ(ENOTDIR, errno);
}
