// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(psignal, example) {
  // Reconfigure stderr to use a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  FILE *fp = fdopen(fds[1], "w");
  ASSERT_NE(NULL, fp);
  fswap(stderr, fp);

  // Write psignal() error messages into one side of the pipe.
  psignal(-1, NULL);
  psignal(SIGSEGV, NULL);
  psignal(SIGBUS, "");
  psignal(SIGFPE, "Hello");

  fswap(stderr, fp);
  ASSERT_EQ(0, fclose(fp));

  // Validate output.
  char buf[77];
  ASSERT_EQ(sizeof(buf) - 1, read(fds[0], buf, sizeof(buf)));
  buf[sizeof(buf) - 1] = '\0';
  ASSERT_STREQ(
      "Unknown signal\n"
      "Segmentation fault\n"
      "Bus error\n"
      "Hello: Floating point exception\n",
      buf);
  ASSERT_EQ(0, close(fds[0]));
}
