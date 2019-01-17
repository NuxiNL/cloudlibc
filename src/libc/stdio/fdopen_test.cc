// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(fdopen, bad) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Bad file descriptor.
  ASSERT_EQ(NULL, fdopen(-1, "r"));
  ASSERT_EQ(EBADF, errno);

  // Invalid mode argument.
  ASSERT_EQ(NULL, fdopen(fd_tmp, "foo"));
  ASSERT_EQ(EINVAL, errno);
}

TEST(fdopen, socket) {
  // Create a socket pair and attach a stdio object to one end.
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
  FILE *file = fdopen(fds[0], "r+b");
  ASSERT_NE(NULL, file);

  // File descriptor should not be seekable.
  ASSERT_EQ(-1, ftello(file));
  ASSERT_EQ(ESPIPE, errno);
  ASSERT_EQ(-1, fseeko(file, 0, SEEK_SET));
  ASSERT_EQ(ESPIPE, errno);

  // Write data into the socket.
  ASSERT_EQ(13, fprintf(file, "%s %s-%d", "Hawaii", "Five", 0));
  ASSERT_EQ('\n', fputc('\n', file));
  ASSERT_EQ(0, fputs("This is an example", file));
  ASSERT_EQ('\n', putc('\n', file));
  ASSERT_EQ(0, fflush(file));

  // Read it back in.
  char buf[34] = {};
  ASSERT_EQ(33, read(fds[1], buf, sizeof(buf)));
  ASSERT_STREQ("Hawaii Five-0\nThis is an example\n", buf);

  // TODO(ed): Add more tests.

  ASSERT_EQ(0, fclose(file));
  ASSERT_EQ(0, close(fds[1]));
}

// TODO(ed): Add more tests.
