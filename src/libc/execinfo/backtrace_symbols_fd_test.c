// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <execinfo.h>
#include <stdio.h>
#include <testing.h>
#include <unistd.h>

TEST(backtrace_symbols_fd, example) {
  int fds[2];
  ASSERT_EQ(0, pipe(fds));

  // Let backtrace_symbols_fd() write its output into a pipe.
  void *input[] = {NULL, lseek, (char *)lseek + 1};
  backtrace_symbols_fd(input, __arraycount(input), fds[1]);
  ASSERT_EQ(0, close(fds[1]));

  // Read output from the pipe and compare it.
  char expected[128];
  size_t len = snprintf(expected, sizeof(expected),
                        "%p\n"
                        "%p == lseek\n"
                        "%p == lseek + 0x1\n",
                        NULL, lseek, (char *)lseek + 1);
  char actual[128];
  ASSERT_EQ(len, read(fds[0], actual, sizeof(actual)));
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_ARREQ(expected, actual, len);
}
