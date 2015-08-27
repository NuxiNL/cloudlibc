// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/ioctl.h>

#include <errno.h>
#include <testing.h>
#include <unistd.h>

TEST(ioctl, bad) {
  // Bad file descriptor.
  int len;
  ASSERT_EQ(-1, ioctl(-1, FIONREAD, &len));
  ASSERT_EQ(EBADF, errno);

  // Bad command.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(-1, ioctl(fds[0], 0xdeadc0de, &len));
  ASSERT_EQ(EINVAL, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

TEST_SEPARATE_PROCESS(ioctl, example) {
  // Bad command.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));

  // No bytes available for reading.
  int len;
  ASSERT_EQ(0, ioctl(fds[0], FIONREAD, &len));
  ASSERT_EQ(0, len);

  // Write data into the pipe. FIONREAD should detect this.
  ASSERT_EQ(5, write(fds[1], "hello", 5));
  ASSERT_EQ(0, ioctl(fds[0], FIONREAD, &len));
  ASSERT_EQ(5, len);

  // Close the write side. The data should still be available for reading.
  ASSERT_EQ(0, close(fds[1]));
  ASSERT_EQ(0, ioctl(fds[0], FIONREAD, &len));
  ASSERT_EQ(5, len);

  // Read the five bytes of data. FIONREAD should drop back to zero.
  char buf[5];
  ASSERT_EQ(sizeof(buf), read(fds[0], buf, sizeof(buf)));
  ASSERT_EQ(0, ioctl(fds[0], FIONREAD, &len));
  ASSERT_EQ(0, len);

  ASSERT_EQ(0, close(fds[0]));
}
