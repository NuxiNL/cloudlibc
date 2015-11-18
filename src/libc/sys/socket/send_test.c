// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <errno.h>
#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

TEST_SEPARATE_PROCESS(send, bad) {
  // Bad file descriptor.
  char b = 'A';
  ASSERT_EQ(-1, send(-1, &b, 1, 0));
  ASSERT_EQ(EBADF, errno);

  // Not connected.
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_EQ(-1, send(fd, &b, 1, 0));
  ASSERT_EQ(ENOTCONN, errno);
  ASSERT_EQ(0, close(fd));

  // Not a socket.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(-1, send(fds[0], &b, 1, 0));
  ASSERT_EQ(ENOTSOCK, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));

  // Bad flags.
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
  ASSERT_EQ(-1, send(fds[0], &b, 1, 0xdeadc0de));
  ASSERT_EQ(EOPNOTSUPP, errno);

  // Close one side of the connection. Sending a packet should fail, but
  // should not generate a SIGPIPE.
  ASSERT_EQ(0, close(fds[1]));
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

// TODO(ed): Add tests for MSG_EOR.
