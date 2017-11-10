// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

TEST(posix_fadvise, bad) {
  ASSERT_EQ(EINVAL, posix_fadvise(123, -1, 10, POSIX_FADV_NORMAL));
  ASSERT_EQ(EINVAL, posix_fadvise(123, 10, -1, POSIX_FADV_NORMAL));
  ASSERT_EQ(EINVAL, posix_fadvise(123, 10, 10, 0xdeadc0de));
  ASSERT_EQ(EBADF, posix_fadvise(-789, 10, 10, POSIX_FADV_NORMAL));
}

TEST(posix_fadvise, example) {
  int fd = openat(fd_tmp, "file", O_CREAT | O_RDWR);
  ASSERT_LE(0, fd);
  ASSERT_EQ(0, posix_fadvise(fd, 10, 10, POSIX_FADV_SEQUENTIAL));
  ASSERT_EQ(0, close(fd));
}
