// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
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
  ASSERT_EQ(-1, getsockopt(-1, SOL_SOCKET, SO_ACCEPTCONN, NULL, &len));
  ASSERT_EQ(EBADF, errno);
  ASSERT_EQ(-1, getsockopt(-1, SOL_SOCKET, SO_ERROR, NULL, &len));
  ASSERT_EQ(EBADF, errno);
  ASSERT_EQ(-1, getsockopt(-1, SOL_SOCKET, SO_TYPE, NULL, &len));
  ASSERT_EQ(EBADF, errno);

  // Not a socket.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(-1, getsockopt(fds[0], SOL_SOCKET, SO_ACCEPTCONN, NULL, &len));
  ASSERT_EQ(ENOTSOCK, errno);
  ASSERT_EQ(-1, getsockopt(fds[0], SOL_SOCKET, SO_ERROR, NULL, &len));
  ASSERT_EQ(ENOTSOCK, errno);
  ASSERT_EQ(-1, getsockopt(fds[0], SOL_SOCKET, SO_TYPE, NULL, &len));
  ASSERT_EQ(ENOTSOCK, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));

  // Bad level.
  ASSERT_EQ(-1, getsockopt(-1, 0xdeadc0de, SO_ERROR, NULL, &len));
  ASSERT_EQ(ENOPROTOOPT, errno);

  // Bad option name.
  ASSERT_EQ(-1, getsockopt(-1, SOL_SOCKET, 0xdeadc0de, NULL, &len));
  ASSERT_EQ(ENOPROTOOPT, errno);
}

TEST(getsockopt, acceptconn) {
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd);

  // Socket should not be accepting incoming connections.
  int acc;
  size_t acclen = sizeof(acc);
  ASSERT_EQ(0, getsockopt(fd, SOL_SOCKET, SO_ACCEPTCONN, &acc, &acclen));
  ASSERT_EQ(sizeof(acc), acclen);
  ASSERT_FALSE(acc);

  // SO_ACCEPTCONN should be true after we call bindat() and listen().
  ASSERT_EQ(0, bindat(fd, fd_tmp, "foo"));
  ASSERT_EQ(0, listen(fd, SOMAXCONN));
  ASSERT_EQ(0, getsockopt(fd, SOL_SOCKET, SO_ACCEPTCONN, &acc, &acclen));
  ASSERT_EQ(sizeof(acc), acclen);
  ASSERT_TRUE(acc);

  ASSERT_EQ(0, close(fd));
}

TEST(getsockopt, error) {
  // Let a socket connect to the other one.
  int fd1 = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd1);
  ASSERT_EQ(0, bindat(fd1, fd_tmp, "hello"));
  ASSERT_EQ(0, listen(fd1, 0));
  int fd2 = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd2);
  ASSERT_EQ(0, connectat(fd2, fd_tmp, "hello"));

  // Error should be zero by default.
  int err;
  size_t errlen = sizeof(err);
  ASSERT_EQ(0, getsockopt(fd2, SOL_SOCKET, SO_ERROR, &err, &errlen));
  ASSERT_EQ(sizeof(err), errlen);
  ASSERT_EQ(0, err);

  // Close the receiving side. The sending side should now see ECONNRESET.
  ASSERT_EQ(0, close(fd1));
  errlen = sizeof(err);
  ASSERT_EQ(0, getsockopt(fd2, SOL_SOCKET, SO_ERROR, &err, &errlen));
  ASSERT_EQ(sizeof(err), errlen);
  ASSERT_EQ(ECONNRESET, err);

  // Error should be cleared by getsockopt().
  errlen = sizeof(err);
  ASSERT_EQ(0, getsockopt(fd2, SOL_SOCKET, SO_ERROR, &err, &errlen));
  ASSERT_EQ(sizeof(err), errlen);
  ASSERT_EQ(0, err);

  ASSERT_EQ(0, close(fd2));
}

TEST(getsockopt, type) {
#define TEST_SOCKTYPE(socktype)                                         \
  do {                                                                  \
    int fd = socket(AF_UNIX, socktype, 0);                              \
    ASSERT_LE(0, fd);                                                   \
    int type;                                                           \
    size_t typelen = sizeof(type);                                      \
    ASSERT_EQ(0, getsockopt(fd, SOL_SOCKET, SO_TYPE, &type, &typelen)); \
    ASSERT_EQ(sizeof(type), typelen);                                   \
    ASSERT_EQ(socktype, type);                                          \
    ASSERT_EQ(0, close(fd));                                            \
  } while (0)
  TEST_SOCKTYPE(SOCK_DGRAM);
  TEST_SOCKTYPE(SOCK_SEQPACKET);
  TEST_SOCKTYPE(SOCK_STREAM);
#undef TEST_SOCKTYPE
}
