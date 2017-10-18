// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <testing.h>
#include <unistd.h>

TEST(write, bad) {
  // Bad file descriptor.
  char b;
  ASSERT_EQ(-1, write(0xdeadc0de, &b, 1));
  ASSERT_EQ(EBADF, errno);

  // Invalid file descriptor type.
  ASSERT_EQ(-1, write(fd_tmp, &b, 1));
  ASSERT_EQ(EBADF, errno);
}

TEST(write, example) {
  // Create pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));

  // Send string across.
  ASSERT_EQ(5, write(fds[1], "Hello", 5));
  char buf[6];
  ASSERT_EQ(5, read(fds[0], buf, sizeof(buf)));
  ASSERT_ARREQ("Hello", buf, 5);

  // Close pipe.
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}
