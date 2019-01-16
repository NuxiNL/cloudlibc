// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/uio.h>

#include <fcntl.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(writev, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Prepare example file.
  int fd = openat(fd_tmp, "file", O_CREAT | O_RDWR);
  ASSERT_NE(-1, fd);

  // Perform a writev().
  char data1[] = "Hello, ";
  char data2[] = "world";
  struct iovec iov[2] = {{.iov_base = data1, .iov_len = sizeof(data1) - 1},
                         {.iov_base = data2, .iov_len = sizeof(data2) - 1}};
  ASSERT_EQ(sizeof(data1) + sizeof(data2) - 2, writev(fd, iov, 2));

  // Read data back.
  char buf[13];
  ASSERT_EQ(sizeof(buf) - 1, pread(fd, buf, sizeof(buf), 0));
  ASSERT_STREQ("Hello, world", buf);

  // Close example file.
  ASSERT_EQ(0, close(fd));
}
