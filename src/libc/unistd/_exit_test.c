// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/procdesc.h>

#include <signal.h>
#include <testing.h>
#include <unistd.h>

TEST(_exit, example) {
  // Fork process.
  int fd;
  int ret = pdfork(&fd);

  // Terminate the child process.
  if (ret == 0)
    _exit(123);

  ASSERT_LT(0, ret);
  siginfo_t si;
  ASSERT_EQ(0, pdwait(fd, &si, 0));
  ASSERT_EQ(0, close(fd));

  // Validate exit code.
  ASSERT_EQ(SIGCHLD, si.si_signo);
  ASSERT_EQ(CLD_EXITED, si.si_code);
  ASSERT_EQ(123, si.si_status);
}
