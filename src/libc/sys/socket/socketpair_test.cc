// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(socketpair, bad) {
  // Invalid address family.
  ASSERT_EQ(-1, socketpair(0xdeadc0de, SOCK_STREAM, 0, NULL));
  ASSERT_EQ(EAFNOSUPPORT, errno);

  // Invalid socket type.
  ASSERT_EQ(-1, socketpair(AF_UNIX, 0xdeadc0de, 0, NULL));
  ASSERT_EQ(EPROTOTYPE, errno);

  // Invalid protocol.
  ASSERT_EQ(-1, socketpair(AF_UNIX, SOCK_STREAM, 0xdeadc0de, NULL));
  ASSERT_EQ(EPROTONOSUPPORT, errno);
}

TEST(socketpair, rights) {
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));

  // Validate default rights on both sides of the socket pair.
  cap_rights_t base, inheriting, all;
  CAP_ALL(&all);
  ASSERT_EQ(0, cap_rights_get_explicit(fds[0], &base, &inheriting));
  ASSERT_EQ(CAP_READ | CAP_WRITE | CAP_FCNTL | CAP_FSTAT | CAP_SOCK_CLIENT |
                CAP_SOCK_SERVER | CAP_EVENT,
            base.__value);
  ASSERT_EQ(all.__value, inheriting.__value);
  ASSERT_EQ(0, cap_rights_get_explicit(fds[1], &base, &inheriting));
  ASSERT_EQ(CAP_READ | CAP_WRITE | CAP_FCNTL | CAP_FSTAT | CAP_SOCK_CLIENT |
                CAP_SOCK_SERVER | CAP_EVENT,
            base.__value);
  ASSERT_EQ(all.__value, inheriting.__value);

  // Validate access modes.
  ASSERT_EQ(O_RDWR, fcntl(fds[0], F_GETFL));
  ASSERT_EQ(O_RDWR, fcntl(fds[1], F_GETFL));

  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

TEST(socketpair, example_stream) {
  // Create socket pair.
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));

  // Test file descriptor type.
  struct stat sb;
  ASSERT_EQ(0, fstat(fds[0], &sb));
  ASSERT_TRUE(S_ISSOCK(sb.st_mode));

  ASSERT_EQ(0, fstat(fds[1], &sb));
  ASSERT_TRUE(S_ISSOCK(sb.st_mode));

  // Send a message into one socket.
  ASSERT_EQ(6, write(fds[0], "Hello ", 6));
  ASSERT_EQ(6, write(fds[0], "world!", 6));

  // Receive it back through the other socket.
  char buf[13];
  ASSERT_EQ(12, read(fds[1], buf, sizeof(buf)));
  buf[12] = '\0';
  ASSERT_STREQ("Hello world!", buf);

  // Close descriptors.
  ASSERT_EQ(0, close(fds[1]));
  ASSERT_EQ(0, close(fds[0]));
}

// TODO(ed): Add tests for SOCK_DGRAM.
