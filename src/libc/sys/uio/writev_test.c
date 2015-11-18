// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/uio.h>

#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

TEST(writev, example) {
  // Prepare example file.
  int fd = openat(fd_tmp, "file", O_CREAT | O_RDWR);
  ASSERT_NE(-1, fd);

  // Perform a writev().
  char data1[7] = "Hello, ";
  char data2[5] = "world";
  struct iovec iov[2] = {{.iov_base = data1, .iov_len = sizeof(data1)},
                         {.iov_base = data2, .iov_len = sizeof(data2)}};
  ASSERT_EQ(sizeof(data1) + sizeof(data2), writev(fd, iov, 2));

  // Read data back.
  char buf[12];
  ASSERT_EQ(sizeof(buf), pread(fd, buf, sizeof(buf), 0));
  ASSERT_ARREQ("Hello, world", buf, __arraycount(buf));

  // Close example file.
  ASSERT_EQ(0, close(fd));
}
