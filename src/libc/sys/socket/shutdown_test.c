// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <errno.h>
#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

TEST(shutdown, bad) {
  // Bad file descriptor.
  ASSERT_EQ(-1, shutdown(-1, SHUT_RDWR));
  ASSERT_EQ(EBADF, errno);

  // Bad shutdown type.
  {
    int fds[2];
    ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
    ASSERT_EQ(-1, shutdown(fds[0], 0xdeadc0de));
    ASSERT_EQ(EINVAL, errno);
    ASSERT_EQ(0, close(fds[0]));
    ASSERT_EQ(0, close(fds[1]));
  }

  // Not a socket.
  {
    ASSERT_EQ(-1, shutdown(fd_tmp, SHUT_RDWR));
    ASSERT_EQ(ENOTSOCK, errno);
  }
}

TEST(shutdown, example) {
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
  ASSERT_EQ(0, fcntl(fds[0], F_SETFL, O_NONBLOCK));

  // Shut down read side.
  char b = 'A';
  ASSERT_EQ(-1, recv(fds[0], &b, 1, 0));
  ASSERT_TRUE(errno == EAGAIN || errno == EWOULDBLOCK);
  ASSERT_EQ(0, shutdown(fds[0], SHUT_RD));
  ASSERT_EQ(0, recv(fds[0], &b, 1, 0));

  // Shut down write side.
  ASSERT_EQ(1, send(fds[0], &b, 1, 0));
  ASSERT_EQ(0, shutdown(fds[0], SHUT_WR));
  ASSERT_EQ(-1, send(fds[0], &b, 1, 0));
  ASSERT_EQ(EPIPE, errno);

  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}
