// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <errno.h>
#include <testing.h>
#include <unistd.h>

TEST(getsockopt, bad) {
  // Bad file descriptor.
  size_t len = 0;
  ASSERT_EQ(-1, getsockopt(-1, SOL_SOCKET, SO_TYPE, NULL, &len));
  ASSERT_EQ(EBADF, errno);

  // Not a socket.
  ASSERT_EQ(-1, getsockopt(fd_tmp, SOL_SOCKET, SO_TYPE, NULL, &len));
  ASSERT_EQ(ENOTSOCK, errno);

  // Bad level.
  ASSERT_EQ(-1, getsockopt(-1, 0xdeadc0de, SO_TYPE, NULL, &len));
  ASSERT_EQ(ENOPROTOOPT, errno);

  // Bad option name.
  ASSERT_EQ(-1, getsockopt(-1, SOL_SOCKET, 0xdeadc0de, NULL, &len));
  ASSERT_EQ(ENOPROTOOPT, errno);
}

TEST(getsockopt, type) {
#define TEST_SOCKTYPE(socktype)                                             \
  do {                                                                      \
    int fds[2];                                                             \
    ASSERT_EQ(0, socketpair(AF_UNIX, socktype, 0, fds));                    \
    int type;                                                               \
    size_t typelen = sizeof(type);                                          \
    ASSERT_EQ(0, getsockopt(fds[0], SOL_SOCKET, SO_TYPE, &type, &typelen)); \
    ASSERT_EQ(sizeof(type), typelen);                                       \
    ASSERT_EQ(socktype, type);                                              \
    ASSERT_EQ(0, close(fds[0]));                                            \
    ASSERT_EQ(0, close(fds[1]));                                            \
  } while (0)
  TEST_SOCKTYPE(SOCK_DGRAM);
  TEST_SOCKTYPE(SOCK_STREAM);
#undef TEST_SOCKTYPE
}
