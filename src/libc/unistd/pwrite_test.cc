// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(pwrite, bad) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Invalid file descriptor.
  char b;
  ASSERT_EQ(-1, pwrite(0xdeadc0de, &b, 1, 123));
  ASSERT_EQ(EBADF, errno);

  // Non-seekable file descriptor.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(-1, pwrite(fds[1], &b, 1, 123));
  ASSERT_EQ(ESPIPE, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));

  // Invalid offset.
  ASSERT_EQ(-1, pwrite(fd_tmp, &b, 1, -12));
  ASSERT_EQ(EINVAL, errno);
}

TEST(pwrite, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Create file for testing.
  int fd = openat(fd_tmp, "test", O_CREAT | O_EXCL | O_RDWR);
  ASSERT_LE(0, fd);

  // Write string at offset and load it back.
  ASSERT_EQ(5, pwrite(fd, "Hello", 5, 3));
  char buf[9];
  ASSERT_EQ(8, read(fd, buf, sizeof(buf)));
  buf[8] = '\0';
  ASSERT_THAT(buf, testing::ElementsAreArray("\0\0\0Hello"));

  // Close file.
  ASSERT_EQ(0, close(fd));
}
