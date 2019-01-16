// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

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
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

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
