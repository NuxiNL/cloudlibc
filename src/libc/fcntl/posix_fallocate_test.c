// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

TEST(posix_fallocate, bad) {
  ASSERT_EQ(EINVAL, posix_fallocate(123, -1, 10));
  ASSERT_EQ(EINVAL, posix_fallocate(123, 10, -1));
  ASSERT_EQ(EBADF, posix_fallocate(-1, 10, 10));
}

TEST(posix_fallocate, example) {
  // Open file and allocate space.
  int fd = openat(fd_tmp, "file", O_CREAT | O_RDWR);
  ASSERT_LE(0, fd);
  ASSERT_EQ(0, posix_fallocate(fd, 10, 12));

  // Check file size.
  struct stat sb;
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_EQ(22, sb.st_size);

  ASSERT_EQ(0, close(fd));
}
