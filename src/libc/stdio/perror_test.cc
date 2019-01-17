// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(perror, example) {
  // Reconfigure stderr to use a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  FILE *fp = fdopen(fds[1], "w");
  ASSERT_NE(NULL, fp);
  fswap(stderr, fp);

  // Write perror() error messages into one side of the pipe.
  errno = -1;
  perror(NULL);
  errno = EINVAL;
  perror(NULL);
  errno = ENOSYS;
  perror("");
  errno = EBADF;
  perror("Hello");

  fswap(stderr, fp);
  ASSERT_EQ(0, fclose(fp));

  // Validate output.
  char buf[84];
  ASSERT_EQ(sizeof(buf) - 1, read(fds[0], buf, sizeof(buf)));
  buf[sizeof(buf) - 1] = '\0';
  ASSERT_STREQ(
      "Unknown error\n"
      "Invalid argument\n"
      "Function not implemented\n"
      "Hello: Bad file descriptor\n",
      buf);
  ASSERT_EQ(0, close(fds[0]));
}
