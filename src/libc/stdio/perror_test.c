// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/procdesc.h>

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <testing.h>
#include <unistd.h>

TEST(perror, example) {
  int fds[2];
  ASSERT_EQ(0, pipe(fds));

  int child;
  int ret = pdfork(&child);
  if (ret == 0) {
    // Write perror() error messages into one side of the pipe.
    stderr = fdopen(fds[1], "w");
    ASSERT_NE(NULL, stderr);

    errno = -1;
    perror(NULL);
    errno = EINVAL;
    perror(NULL);
    errno = ENOSYS;
    perror("");
    errno = EBADF;
    perror("Hello");
    fflush(stderr);
    _Exit(123);
  }

  // Wait for the child process to terminate.
  ASSERT_LT(0, ret);
  ASSERT_EQ(0, close(fds[1]));
  siginfo_t si;
  ASSERT_EQ(0, pdwait(child, &si, 0));
  ASSERT_EQ(0, close(child));

  // Validate exit code.
  ASSERT_EQ(SIGCHLD, si.si_signo);
  ASSERT_EQ(CLD_EXITED, si.si_code);
  ASSERT_EQ(123, si.si_status);

  // Validate output.
  char buf[84];
  ASSERT_EQ(83, read(fds[0], buf, sizeof(buf)));
  ASSERT_ARREQ(
      "Unknown error\n"
      "Invalid argument\n"
      "Function not implemented\n"
      "Hello: Bad file descriptor\n",
      buf, __arraycount(buf));

  ASSERT_EQ(0, close(fds[0]));
}
