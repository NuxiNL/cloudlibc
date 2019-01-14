// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(raise, bad) {
  // Negative signal number.
  ASSERT_EQ(-1, raise(-1));
  ASSERT_EQ(EINVAL, errno);

  // Signal out of bounds.
  ASSERT_EQ(-1, raise(NSIG));
  ASSERT_EQ(EINVAL, errno);
}

TEST(raise, zero) {
  // Signal zero can be used to check if the other process is still
  // running. It makes little sense for raise(), though.
  ASSERT_EQ(0, raise(0));
}

#if 0  // TODO(ed): How to test this without forking?
static void test_signal(int sig, int code) {
  SCOPED_NOTE(sig, {
    // Spawn a child process.
    int fd;
    int ret = pdfork(&fd);

    if (ret == 0) {
      // In the child process, raise a signal.
      raise(sig);
      _Exit(42);
    }

    // In the parent process, wait for the child process to die.
    ASSERT_LT(0, ret);
    siginfo_t si;
    ASSERT_EQ(0, pdwait(fd, &si, 0));
    ASSERT_EQ(0, close(fd));

    // Validate return code.
    ASSERT_EQ(SIGCHLD, si.si_signo);
    ASSERT_EQ(code, si.si_code);
    ASSERT_EQ(code == CLD_KILLED ? sig : 42, si.si_status);
  });
}

TEST(raise, allsigs) {
  // Our raise() always behaves using the default signal actions, except
  // that SIGPIPE is ignored.
  test_signal(SIGABRT, CLD_KILLED);
  test_signal(SIGALRM, CLD_KILLED);
  test_signal(SIGBUS, CLD_KILLED);
  test_signal(SIGCHLD, CLD_EXITED);
  test_signal(SIGCONT, CLD_EXITED);
  test_signal(SIGFPE, CLD_KILLED);
  test_signal(SIGHUP, CLD_KILLED);
  test_signal(SIGILL, CLD_KILLED);
  test_signal(SIGINT, CLD_KILLED);
  test_signal(SIGKILL, CLD_KILLED);
  test_signal(SIGPIPE, CLD_EXITED);
  test_signal(SIGQUIT, CLD_KILLED);
  test_signal(SIGSEGV, CLD_KILLED);
  test_signal(SIGTERM, CLD_KILLED);
  test_signal(SIGUSR1, CLD_KILLED);
  test_signal(SIGUSR2, CLD_KILLED);
  test_signal(SIGSYS, CLD_KILLED);
  test_signal(SIGTRAP, CLD_KILLED);
  test_signal(SIGURG, CLD_EXITED);
  test_signal(SIGVTALRM, CLD_KILLED);
  test_signal(SIGXCPU, CLD_KILLED);
  test_signal(SIGXFSZ, CLD_KILLED);
}
#endif
