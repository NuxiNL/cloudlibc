// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/uio.h>

#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

TEST(readv, example) {
  // Prepare example file.
  int fd = openat(fd_tmp, "file", O_CREAT | O_RDWR);
  ASSERT_NE(-1, fd);
  ASSERT_EQ(12, write(fd, "Hello, world", 12));

  // Seek back.
  ASSERT_EQ(2, lseek(fd, 2, SEEK_SET));

  // Perform a readv().
  char data1[5];
  char data2[3];
  struct iovec iov[2] = {{.iov_base = data1, .iov_len = sizeof(data1)},
                         {.iov_base = data2, .iov_len = sizeof(data2)}};
  ASSERT_EQ(sizeof(data1) + sizeof(data2), readv(fd, iov, 2));
  ASSERT_ARREQ("llo, ", data1, __arraycount(data1));
  ASSERT_ARREQ("wor", data2, __arraycount(data2));

  // Close example file.
  ASSERT_EQ(0, close(fd));
}
