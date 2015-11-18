// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <errno.h>
#include <testing.h>
#include <unistd.h>

TEST(listen, bad) {
  ASSERT_EQ(-1, listen(-1, 0));
  ASSERT_EQ(EBADF, errno);

  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(-1, listen(fds[0], 0));
  ASSERT_EQ(ENOTSOCK, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));

  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
  ASSERT_EQ(-1, listen(fds[0], 0));
  ASSERT_EQ(EINVAL, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));

  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd);
  ASSERT_EQ(-1, listen(fd, 0));
  ASSERT_EQ(EDESTADDRREQ, errno);
  ASSERT_EQ(0, close(fd));
}

TEST(listen, example) {
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd);
  ASSERT_EQ(0, bindat(fd, fd_tmp, "foo"));
  ASSERT_EQ(0, listen(fd, 0));
  ASSERT_EQ(0, close(fd));
}
