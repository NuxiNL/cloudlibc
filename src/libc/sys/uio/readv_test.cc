// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/uio.h>

#include <fcntl.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(readv, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Prepare example file.
  int fd = openat(fd_tmp, "file", O_CREAT | O_RDWR);
  ASSERT_NE(-1, fd);
  ASSERT_EQ(12, write(fd, "Hello, world", 12));

  // Seek back.
  ASSERT_EQ(2, lseek(fd, 2, SEEK_SET));

  // Perform a readv().
  char data1[6];
  char data2[4];
  struct iovec iov[2] = {{.iov_base = data1, .iov_len = sizeof(data1) - 1},
                         {.iov_base = data2, .iov_len = sizeof(data2) - 1}};
  ASSERT_EQ(sizeof(data1) + sizeof(data2) - 2, readv(fd, iov, 2));
  data1[5] = '\0';
  ASSERT_STREQ("llo, ", data1);
  data1[3] = '\0';
  ASSERT_STREQ("wor", data2);

  // Close example file.
  ASSERT_EQ(0, close(fd));
}
