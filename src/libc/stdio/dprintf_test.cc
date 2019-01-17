// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(dprintf, bad) {
  // Bad file descriptor.
  ASSERT_EQ(-1, dprintf(-1, "Hello, world\n"));
  ASSERT_EQ(EBADF, errno);

  // The encoding error should already occur before writing the first byte.
  ASSERT_EQ(-1, dprintf(-1, "%ls\n", L"ï"));
  ASSERT_EQ(EILSEQ, errno);

  // Write to descriptor until it would block.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, fcntl(fds[1], F_SETFL, O_NONBLOCK));
  ASSERT_EQ(-1, dprintf(fds[1], "%1000000000s", ""));
  ASSERT_EQ(EAGAIN, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));

  // Other side of the pipe has been closed.
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(-1, dprintf(fds[1], "Hello"));
  ASSERT_EQ(EPIPE, errno);
  ASSERT_EQ(0, close(fds[1]));
}

TEST(dprintf, example) {
  // Write a string into a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(12, dprintf(fds[1], "%s, %s", "Hello", "world"));
  ASSERT_EQ(0, close(fds[1]));

  // Read it back again.
  char buf[13];
  ASSERT_EQ(12, read(fds[0], buf, sizeof(buf)));
  buf[12] = '\0';
  ASSERT_STREQ("Hello, world", buf);
  ASSERT_EQ(0, close(fds[0]));
}
