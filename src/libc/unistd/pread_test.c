// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

TEST(pread, bad) {
  // Invalid file descriptor.
  char b;
  ASSERT_EQ(-1, pread(0xdeadc0de, &b, 1, 123));
  ASSERT_EQ(EBADF, errno);

  // Invalid offset.
  ASSERT_EQ(-1, pread(fd_tmp, &b, 1, -12));
  ASSERT_EQ(EINVAL, errno);
}

TEST(pread, example) {
  // Create file for testing.
  int fd = openat(fd_tmp, "test", O_CREAT | O_EXCL | O_RDWR);
  ASSERT_LE(0, fd);

  // Write string and load it back from an offset.
  ASSERT_EQ(12, write(fd, "Hello, world", 12));
  char buf[6];
  ASSERT_EQ(5, pread(fd, buf, sizeof(buf), 7));
  ASSERT_ARREQ("world", buf, 5);

  // Close file.
  ASSERT_EQ(0, close(fd));
}
