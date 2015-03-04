// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <errno.h>
#include <testing.h>
#include <unistd.h>

TEST(socket, bad) {
  // Invalid address family.
  ASSERT_EQ(-1, socket(0xdeadc0de, SOCK_STREAM, 0));
  ASSERT_EQ(EAFNOSUPPORT, errno);

  // Invalid socket type.
  ASSERT_EQ(-1, socket(AF_UNIX, 0xdeadc0de, 0));
  ASSERT_EQ(EPROTOTYPE, errno);

  // Invalid protocol.
  ASSERT_EQ(-1, socket(AF_UNIX, SOCK_STREAM, 0xdeadc0de));
  ASSERT_EQ(EPROTONOSUPPORT, errno);
}

TEST(socket, example) {
  // Create a new socket.
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd);
  ASSERT_EQ(0, close(fd));
}
