// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <testing.h>
#include <unistd.h>

TEST(recvmsg, bad) {
  // Bad file descriptor.
  char b;
  struct iovec biov = {.iov_base = &b, .iov_len = 1};
  struct msghdr message = {
      .msg_iov = &biov, .msg_iovlen = 1,
  };
  ASSERT_EQ(-1, recvmsg(-1, &message, 0));
  ASSERT_EQ(EBADF, errno);

  // Not connected.
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_EQ(-1, recvmsg(fd, &message, 0));
  ASSERT_EQ(ENOTCONN, errno);
  ASSERT_EQ(0, close(fd));

  // Not a socket.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(-1, recvmsg(fds[0], &message, 0));
  ASSERT_EQ(ENOTSOCK, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));

  // Blocking recvmsg() on non-blocking file descriptor.
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
  ASSERT_EQ(0, fcntl(fds[0], F_SETFL, O_NONBLOCK));
  ASSERT_EQ(-1, recvmsg(fds[0], &message, 0));
  ASSERT_TRUE(errno == EAGAIN || errno == EWOULDBLOCK);

  // Bad flags.
  ASSERT_EQ(-1, recvmsg(fds[0], &message, 0xdeadc0de));
  ASSERT_EQ(EOPNOTSUPP, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

// TODO(ed): Add more tests.
