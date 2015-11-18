// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

TEST(mkfifoat, simple) {
  // Create a FIFO.
  ASSERT_EQ(0, mkfifoat(fd_tmp, "example"));

  struct stat sb;
  ASSERT_EQ(0, fstatat(fd_tmp, "example", &sb, 0));
  ASSERT_TRUE(S_ISFIFO(sb.st_mode));

  // Open the FIFO for reading and writing.
  int fd_read = openat(fd_tmp, "example", O_RDONLY | O_NONBLOCK);
  ASSERT_LE(0, fd_read);
  ASSERT_EQ(0, fstat(fd_read, &sb));
  ASSERT_TRUE(S_ISFIFO(sb.st_mode));

  int fd_write = openat(fd_tmp, "example", O_WRONLY);
  ASSERT_LE(0, fd_write);
  ASSERT_EQ(0, fstat(fd_write, &sb));
  ASSERT_TRUE(S_ISFIFO(sb.st_mode));

  // Write data into it and extract it again.
  ASSERT_EQ(5, write(fd_write, "Hello", 5));

  char buf[6];
  ASSERT_EQ(5, read(fd_read, buf, sizeof(buf)));
  ASSERT_ARREQ("Hello", buf, 5);

  // Close the FIFO.
  ASSERT_EQ(0, close(fd_read));
  ASSERT_EQ(0, close(fd_write));
}

TEST(mkfifoat, empty_pathname) {
  ASSERT_EQ(-1, mkfifoat(fd_tmp, ""));
  ASSERT_EQ(ENOENT, errno);
}
