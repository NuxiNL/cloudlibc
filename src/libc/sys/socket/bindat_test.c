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
  ASSERT_EQ(EBADF, bindat(-1, fd_tmp, "example"));

  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd);
  ASSERT_EQ(ENOENT, bindat(fd, fd_tmp, ""));
  ASSERT_EQ(ENOTCAPABLE, bindat(fd, fd_tmp, "../sock"));
  ASSERT_EQ(0, close(fd));
}

TEST(bindat, success) {
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd);

  // We should be able to bind the socket once.
  ASSERT_EQ(0, bindat(fd, fd_tmp, "foo"));
  ASSERT_EQ(EINVAL, bindat(fd, fd_tmp, "bar"));

  // Created file should correspond with the socket.
  struct stat sb;
  ASSERT_EQ(0, fstatat(fd_tmp, "foo", &sb, 0));
  ASSERT_TRUE(S_ISSOCK(sb.st_mode));

  ASSERT_EQ(0, close(fd));
}
