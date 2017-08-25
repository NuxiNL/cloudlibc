// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
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

  // Not a socket.
  int fd = kqueue();
  ASSERT_EQ(-1, accept(fd, NULL, NULL));
  ASSERT_EQ(ENOTSOCK, errno);
  ASSERT_EQ(0, close(fd));
}
