// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

TEST(fcntl, bad) {
  ASSERT_EQ(-1, fcntl(0xdeadc0de, 0xdeadc0de));
  ASSERT_EQ(EINVAL, errno);
}

TEST(fcntl, getfd_setfd) {
  // There is no close-on-exec flag in this environment, as the
  // traditional execve() function does not exist. Just return bogus
  // values.
  ASSERT_EQ(FD_CLOEXEC, fcntl(0xdeadc0de, F_GETFD));

  ASSERT_EQ(0, fcntl(0xdeadc0de, F_SETFD, 0));
  ASSERT_EQ(0, fcntl(0xdeadc0de, F_SETFD, FD_CLOEXEC));
}

TEST(fcntl, getfl_setfl_bad) {
  ASSERT_EQ(-1, fcntl(0xdeadc0de, F_GETFL));
  ASSERT_EQ(EBADF, errno);
  ASSERT_EQ(-1, fcntl(0xdeadc0de, F_SETFL, 0));
  ASSERT_EQ(EBADF, errno);
}

TEST(fcntl, getfl_setfl_example) {
  // Open a file and attempt to re-obtain the open flags. Flags that
  // don't just apply to opening should be preserved.
  int fd = openat(fd_tmp, "file", O_WRONLY | O_NONBLOCK | O_CREAT | O_EXCL);
  ASSERT_LE(0, fd);
  ASSERT_EQ(O_WRONLY | O_NONBLOCK, fcntl(fd, F_GETFL));

  // Turn off non-blocking mode. Turn on append mode.
  ASSERT_EQ(0, fcntl(fd, F_SETFL, O_RDWR | O_EXCL | O_APPEND));
  ASSERT_EQ(O_WRONLY | O_APPEND, fcntl(fd, F_GETFL));

  ASSERT_EQ(0, close(fd));
}
