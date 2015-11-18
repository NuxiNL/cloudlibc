// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdio.h>
#include <testing.h>
#include <unistd.h>

TEST_SEPARATE_PROCESS(perror, example) {
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
  fflush(stderr);

  // Validate output.
  char buf[84];
  ASSERT_EQ(sizeof(buf) - 1, read(fds[0], buf, sizeof(buf)));
  ASSERT_ARREQ(
      "Unknown error\n"
      "Invalid argument\n"
      "Function not implemented\n"
      "Hello: Bad file descriptor\n",
      buf, sizeof(buf) - 1);
}
