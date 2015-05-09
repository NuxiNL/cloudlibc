// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>
#include <sys/socket.h>

#include <errno.h>
#include <fcntl.h>
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

TEST(socket, rights) {
  // Create a new socket.
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_LE(0, fd);

  // Validate rights.
  cap_rights_t base, inheriting, all;
  CAP_ALL(&all);
  ASSERT_EQ(0, cap_rights_get_explicit(fd, &base, &inheriting));
  ASSERT_EQ(CAP_READ | CAP_WRITE | CAP_FCNTL | CAP_FSTAT | CAP_SOCK_CLIENT |
                CAP_SOCK_SERVER | CAP_EVENT,
            base.__value);
  ASSERT_EQ(all.__value, inheriting.__value);

  // Validate access modes.
  ASSERT_EQ(O_RDWR, fcntl(fd, F_GETFL));
  ASSERT_EQ(0, close(fd));
}
