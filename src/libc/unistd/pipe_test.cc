// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>
#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(pipe, readwrite) {
  // Create pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));

  // Validate stat structures.
  struct stat sb;
  ASSERT_EQ(0, fstat(fds[0], &sb));
  ASSERT_TRUE(S_ISFIFO(sb.st_mode));
  ASSERT_EQ(0, fstat(fds[1], &sb));
  ASSERT_TRUE(S_ISFIFO(sb.st_mode));

  // Write data into it.
  ASSERT_EQ(5, write(fds[1], "Hello", 5));

  // Extract data again.
  char buf[6];
  ASSERT_EQ(5, read(fds[0], buf, sizeof(buf)));
  buf[5] = '\0';
  ASSERT_STREQ("Hello", buf);

  // Close read end. Write should return EPIPE, but a signal should not
  // be delivered.
  ASSERT_EQ(0, close(fds[0]));
  char f = '!';
  ASSERT_EQ(-1, write(fds[1], &f, 1));
  ASSERT_EQ(EPIPE, errno);

  ASSERT_EQ(0, close(fds[1]));
}

TEST(pipe, rights) {
  int fds[2];
  ASSERT_EQ(0, pipe(fds));

  // Validate default rights on both sides of the pipe.
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
