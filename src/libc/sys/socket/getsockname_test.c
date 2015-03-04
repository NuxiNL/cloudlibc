// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>
#include <sys/un.h>

#include <errno.h>
#include <testing.h>
#include <unistd.h>

TEST(getsockname, bad) {
  // Bad file descriptor.
  ASSERT_EQ(-1, getsockname(-1, NULL, NULL));
  ASSERT_EQ(EBADF, errno);

  // Not a socket.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(-1, getsockname(fds[0], NULL, NULL));
  ASSERT_EQ(ENOTSOCK, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

TEST(getsockname, example) {
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));

  // Zero-sized output buffer.
  size_t len = 0;
  ASSERT_EQ(0, getsockname(fds[0], (struct sockaddr *)42, &len));
  ASSERT_EQ(sizeof(struct sockaddr_un), len);

  // Proper output buffer.
  struct sockaddr_un sun;
  len = sizeof(sun);
  ASSERT_EQ(0, getsockname(fds[0], (struct sockaddr *)&sun, &len));
  ASSERT_EQ(sizeof(sun), len);
  ASSERT_EQ(AF_UNIX, sun.sun_family);

  // Generic socket address storage.
  struct sockaddr_storage ss;
  len = sizeof(ss);
  ASSERT_EQ(0, getsockname(fds[0], (struct sockaddr *)&ss, &len));
  ASSERT_EQ(sizeof(struct sockaddr_un), len);
  ASSERT_EQ(AF_UNIX, ss.ss_family);

  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}
