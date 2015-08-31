// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>
#include <sys/stat.h>

#include <errno.h>
#include <testing.h>
#include <unistd.h>

TEST(bindat, failure) {
  // Bad file descriptor.
  ASSERT_EQ(-1, bindat(-1, fd_tmp, "example"));
  ASSERT_EQ(EBADF, errno);

  // Target already exists.
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd);
  ASSERT_EQ(0, mkfifoat(fd_tmp, "fifo"));
  ASSERT_EQ(-1, bindat(fd, fd_tmp, "fifo"));
  ASSERT_EQ(EADDRINUSE, errno);

  // Not a directory/socket.
  ASSERT_EQ(-1, bindat(fd, fd, "example"));
  ASSERT_EQ(ENOTDIR, errno);
  ASSERT_EQ(-1, bindat(fd_tmp, fd_tmp, "example"));
  ASSERT_EQ(ENOTSOCK, errno);

  // Empty filename.
  ASSERT_EQ(-1, bindat(fd, fd_tmp, ""));
  ASSERT_EQ(ENOENT, errno);

  // Bad pathname.
  ASSERT_EQ(-1, bindat(fd, fd_tmp, "../sock"));
  ASSERT_EQ(ENOTCAPABLE, errno);
  ASSERT_EQ(-1, bindat(fd, fd_tmp, "/sock"));
  ASSERT_EQ(ENOTCAPABLE, errno);
  ASSERT_EQ(0, close(fd));
}

TEST(bindat, success) {
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd);

  // We should be able to bind the socket once.
  ASSERT_EQ(0, bindat(fd, fd_tmp, "foo"));
  ASSERT_EQ(-1, bindat(fd, fd_tmp, "bar"));
  ASSERT_EQ(EINVAL, errno);

  // Created file should correspond with the socket.
  struct stat sb;
  ASSERT_EQ(0, fstatat(fd_tmp, "foo", &sb, 0));
  ASSERT_TRUE(S_ISSOCK(sb.st_mode));

  ASSERT_EQ(0, close(fd));
}
