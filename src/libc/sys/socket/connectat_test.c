// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <errno.h>
#include <testing.h>
#include <unistd.h>

TEST(connectat, failure) {
  ASSERT_EQ(-1, connectat(-1, fd_tmp, "example"));
  ASSERT_EQ(EBADF, errno);

  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd);
  ASSERT_EQ(-1, connectat(fd, fd_tmp, ""));
  ASSERT_EQ(ENOENT, errno);
  ASSERT_EQ(-1, connectat(fd, fd_tmp, "../sock"));
  ASSERT_EQ(ENOTCAPABLE, errno);

  // Not a directory/socket.
  ASSERT_EQ(-1, bindat(fd, fd, "example"));
  ASSERT_EQ(ENOTDIR, errno);
  ASSERT_EQ(-1, bindat(fd_tmp, fd_tmp, "example"));
  ASSERT_EQ(ENOTSOCK, errno);
  ASSERT_EQ(0, close(fd));
}

TEST(connectat, success) {
  // Create a socket to which we can connect.
  int fd1 = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_EQ(0, bindat(fd1, fd_tmp, "foo"));

  // If we don't call listen(), we can't connect to the socket.
  int fd2 = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_EQ(-1, connectat(fd2, fd_tmp, "foo"));
  ASSERT_EQ(ECONNREFUSED, errno);

  // After we start listening, we can connect to it.
  ASSERT_EQ(0, listen(fd1, SOMAXCONN));
  ASSERT_EQ(0, connectat(fd2, fd_tmp, "foo"));
  ASSERT_EQ(-1, connectat(fd2, fd_tmp, "foo"));
  ASSERT_EQ(EISCONN, errno);

  ASSERT_EQ(0, close(fd1));
  ASSERT_EQ(0, close(fd2));
}
