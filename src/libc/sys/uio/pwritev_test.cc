// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/uio.h>

#include <fcntl.h>
#include <unistd.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(pwritev, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Prepare example file.
  int fd = openat(fd_tmp, "file", O_CREAT | O_RDWR);
  ASSERT_NE(-1, fd);

  // Perform a pwritev().
  char data1[] = "Hello, ";
  char data2[] = "world";
  struct iovec iov[2] = {{.iov_base = data1, .iov_len = sizeof(data1) - 1},
                         {.iov_base = data2, .iov_len = sizeof(data2) - 1}};
  ASSERT_EQ(sizeof(data1) + sizeof(data2) - 2, pwritev(fd, iov, 2, 2));

  // Read data back.
  char buf[15];
  ASSERT_EQ(sizeof(buf) - 1, read(fd, buf, sizeof(buf)));
  buf[14] = '\0';
  ASSERT_THAT(buf, testing::ElementsAreArray("\0\0Hello, world"));

  // Close example file.
  ASSERT_EQ(0, close(fd));
}
