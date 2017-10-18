// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
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
      .msg_iov = &biov,
      .msg_iovlen = 1,
  };
  ASSERT_EQ(-1, recvmsg(-1, &message, 0));
  ASSERT_EQ(EBADF, errno);

  // Not a socket.
  {
    ASSERT_EQ(-1, recvmsg(fd_tmp, &message, 0));
    ASSERT_EQ(ENOTSOCK, errno);
  }

  {
    // Blocking recvmsg() on non-blocking file descriptor.
    int fds[2];
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
}

TEST(recvmsg, msg_trunc) {
  // Prepare a socket with a five-byte message.
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_DGRAM, 0, fds));
  ASSERT_EQ(5, send(fds[0], "Hello", 5, 0));

  // Four-byte read should return a truncated message with MSG_TRUNC set.
  {
    char buf[4];
    struct iovec iov = {.iov_base = buf, .iov_len = sizeof(buf)};
    struct msghdr msg = {
        .msg_iov = &iov,
        .msg_iovlen = 1,
    };
    ASSERT_EQ(sizeof(buf), recvmsg(fds[1], &msg, MSG_PEEK));
    ASSERT_ARREQ("Hell", buf, 4);
    ASSERT_EQ(MSG_TRUNC, msg.msg_flags);
  }

  // Five-byte read should return the entire message with MSG_TRUNC clear.
  {
    char buf[5];
    struct iovec iov = {.iov_base = buf, .iov_len = sizeof(buf)};
    struct msghdr msg = {
        .msg_iov = &iov,
        .msg_iovlen = 1,
    };
    ASSERT_EQ(sizeof(buf), recvmsg(fds[1], &msg, 0));
    ASSERT_ARREQ("Hello", buf, 5);
    ASSERT_EQ(0, msg.msg_flags);
  }

  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

// TODO(ed): Add more tests.
