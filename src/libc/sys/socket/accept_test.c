// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/event.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <errno.h>
#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

TEST(accept, bad) {
  // Bad descriptor.
  ASSERT_EQ(-1, accept(-1, NULL, NULL));
  ASSERT_EQ(EBADF, errno);

  // listen() has not been called.
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_EQ(-1, accept(fd, NULL, NULL));
  ASSERT_EQ(EINVAL, errno);
  ASSERT_EQ(0, close(fd));

  // Not a socket.
  fd = kqueue();
  ASSERT_EQ(-1, accept(fd, NULL, NULL));
  ASSERT_EQ(ENOTSOCK, errno);
  ASSERT_EQ(0, close(fd));
}

TEST(accept, example_null) {
  // Create a socket on which we can call accept().
  int fd1 = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd1);
  ASSERT_EQ(0, bindat(fd1, fd_tmp, "foo"));
  ASSERT_EQ(0, listen(fd1, 1));

  // Test O_NONBLOCK.
  ASSERT_EQ(0, fcntl(fd1, F_SETFL, O_NONBLOCK));
  ASSERT_EQ(-1, accept(fd1, NULL, NULL));
  ASSERT_TRUE(errno == EAGAIN || errno == EWOULDBLOCK);

  // Connect.
  int fd2 = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd2);
  ASSERT_EQ(0, connectat(fd2, fd_tmp, "foo"));

  // Accept connection, but discard the peer address.
  int fd3 = accept(fd1, NULL, NULL);
  ASSERT_LE(0, fd3);

  // Send a message into one socket.
  ASSERT_EQ(6, write(fd2, "Hello ", 6));
  ASSERT_EQ(6, write(fd2, "world!", 6));

  // Receive it back through the other socket.
  char buf[13];
  ASSERT_EQ(12, read(fd3, buf, sizeof(buf)));
  ASSERT_ARREQ("Hello world!", buf, 12);

  ASSERT_EQ(0, close(fd1));
  ASSERT_EQ(0, close(fd2));
  ASSERT_EQ(0, close(fd3));
}

TEST(accept, example_zero) {
  // Create a socket on which we can call accept().
  int fd1 = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd1);
  ASSERT_EQ(0, bindat(fd1, fd_tmp, "foo"));
  ASSERT_EQ(0, listen(fd1, 1));

  // Connect.
  int fd2 = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd2);
  ASSERT_EQ(0, connectat(fd2, fd_tmp, "foo"));

  // Accept connection, but use a zero-length socket address.
  size_t slen = 0;
  int fd3 = accept(fd1, (struct sockaddr *)42, &slen);
  ASSERT_LE(0, fd3);

  ASSERT_EQ(0, close(fd1));
  ASSERT_EQ(0, close(fd2));
  ASSERT_EQ(0, close(fd3));
}

TEST(accept, example_sockaddr_un) {
  // Create a socket on which we can call accept().
  int fd1 = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd1);
  ASSERT_EQ(0, bindat(fd1, fd_tmp, "foo"));
  ASSERT_EQ(0, listen(fd1, 1));

  // Connect.
  int fd2 = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd2);
  ASSERT_EQ(0, connectat(fd2, fd_tmp, "foo"));

  // Accept connection and fetch the socket address.
  struct sockaddr_un sun;
  size_t slen = sizeof(sun);
  int fd3 = accept(fd1, (struct sockaddr *)&sun, &slen);
  ASSERT_LE(0, fd3);
  ASSERT_EQ(sizeof(sun), slen);
  ASSERT_EQ(AF_UNIX, sun.sun_family);

  ASSERT_EQ(0, close(fd1));
  ASSERT_EQ(0, close(fd2));
  ASSERT_EQ(0, close(fd3));
}
