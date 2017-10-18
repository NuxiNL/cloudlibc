// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <errno.h>
#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

TEST(send, bad) {
  // Bad file descriptor.
  char b = 'A';
  ASSERT_EQ(-1, send(-1, &b, 1, 0));
  ASSERT_EQ(EBADF, errno);

  // Not a socket.
  {
    ASSERT_EQ(-1, send(fd_tmp, &b, 1, 0));
    ASSERT_EQ(ENOTSOCK, errno);
  }

  // Bad flags.
  {
    int fds[2];
    ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
    ASSERT_EQ(-1, send(fds[0], &b, 1, 0xdeadc0de));
    ASSERT_EQ(EOPNOTSUPP, errno);
    ASSERT_EQ(0, close(fds[0]));
    ASSERT_EQ(0, close(fds[1]));
  }
}

TEST_SEPARATE_PROCESS(send, epipe) {
  // Close one side of the connection. Sending a packet should fail, but
  // should not generate a SIGPIPE.
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
  ASSERT_EQ(0, close(fds[1]));

  char b = 'A';
  ASSERT_EQ(-1, send(fds[0], &b, 1, 0));
  ASSERT_EQ(EPIPE, errno);
  ASSERT_EQ(0, close(fds[0]));
}

TEST(send, example) {
  // Create socket pair.
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));

  // Send a message into one socket.
  ASSERT_EQ(6, send(fds[0], "Hello ", 6, 0));
  ASSERT_EQ(6, send(fds[0], "world!", 6, 0));

  // Receive it back through the other socket.
  char buf[13];
  ASSERT_EQ(12, read(fds[1], buf, sizeof(buf)));
  ASSERT_ARREQ("Hello world!", buf, 12);

  // Destroy socketpair.
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}
