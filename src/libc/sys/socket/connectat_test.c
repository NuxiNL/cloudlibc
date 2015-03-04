// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <errno.h>
#include <testing.h>
#include <unistd.h>

TEST(connectat, failure) {
  ASSERT_EQ(EBADF, connectat(-1, fd_tmp, "example"));

  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd);
  ASSERT_EQ(ENOENT, connectat(fd, fd_tmp, ""));
  ASSERT_EQ(ENOTCAPABLE, connectat(fd, fd_tmp, "../sock"));
  ASSERT_EQ(0, close(fd));
}

TEST(connectat, success) {
  // Create a socket to which we can connect.
  int fd1 = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_EQ(0, bindat(fd1, fd_tmp, "foo"));

  // If we don't call listen(), we can't connect to the socket.
  int fd2 = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_EQ(ECONNREFUSED, connectat(fd2, fd_tmp, "foo"));

  // After we start listening, we can connect to it.
  ASSERT_EQ(0, listen(fd1, SOMAXCONN));
  ASSERT_EQ(0, connectat(fd2, fd_tmp, "foo"));
  ASSERT_EQ(EISCONN, connectat(fd2, fd_tmp, "foo"));

  ASSERT_EQ(0, close(fd1));
  ASSERT_EQ(0, close(fd2));
}
