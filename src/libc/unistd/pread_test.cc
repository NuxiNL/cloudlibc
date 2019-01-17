// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(pread, bad) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Invalid file descriptor.
  char b;
  ASSERT_EQ(-1, pread(0xdeadc0de, &b, 1, 123));
  ASSERT_EQ(EBADF, errno);

  // Non-seekable file descriptor.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(-1, pread(fds[0], &b, 1, 123));
  ASSERT_EQ(ESPIPE, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));

  // Invalid offset.
  ASSERT_EQ(-1, pread(fd_tmp, &b, 1, -12));
  ASSERT_EQ(EINVAL, errno);
}

TEST(pread, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Create file for testing.
  int fd = openat(fd_tmp, "test", O_CREAT | O_EXCL | O_RDWR);
  ASSERT_LE(0, fd);

  // Write string and load it back from an offset.
  ASSERT_EQ(12, write(fd, "Hello, world", 12));
  char buf[6];
  ASSERT_EQ(5, pread(fd, buf, sizeof(buf), 7));
  buf[5] = '\0';
  ASSERT_STREQ("world", buf);

  // Close file.
  ASSERT_EQ(0, close(fd));
}

TEST(pread, eof) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Create file.
  int fd = openat(fd_tmp, "Hello", O_CREAT | O_RDWR);
  ASSERT_EQ(6, write(fd, "foobar", 6));

  // Read from past the end of the file. This should return EOF (0).
  char buf[2];
  ASSERT_EQ(0, pread(fd, buf, sizeof(buf), 10));

  // Close file.
  ASSERT_EQ(0, close(fd));
}
