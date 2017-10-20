// Copyright (c) 2016-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <signal.h>
#include <stdio.h>
#include <testing.h>
#include <unistd.h>

TEST_SINGLE_THREADED(psiginfo, example) {
  // Reconfigure stderr to use a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  FILE *fp = fdopen(fds[1], "w");
  ASSERT_NE(NULL, fp);
  fswap(stderr, fp);

  // Write psiginfo() error messages into one side of the pipe.
  siginfo_t info = {.si_signo = -1};
  psiginfo(&info, NULL);
  info.si_signo = SIGSEGV;
  psiginfo(&info, NULL);
  info.si_signo = SIGBUS;
  psiginfo(&info, "");
  info.si_signo = SIGFPE;
  psiginfo(&info, "Hello");

  fswap(stderr, fp);
  ASSERT_EQ(0, fclose(fp));

  // Validate output.
  char buf[77];
  ASSERT_EQ(sizeof(buf) - 1, read(fds[0], buf, sizeof(buf)));
  ASSERT_ARREQ(
      "Unknown signal\n"
      "Segmentation fault\n"
      "Bus error\n"
      "Hello: Floating point exception\n",
      buf, sizeof(buf) - 1);
  ASSERT_EQ(0, close(fds[0]));
}
