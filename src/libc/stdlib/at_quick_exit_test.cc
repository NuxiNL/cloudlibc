// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "gtest/gtest.h"

#if 0  // TODO(ed): How to test this without forking?
static int val = 1;

static void handler1(void) {
  ASSERT_EQ(1, val);
  val = 2;
}

static void handler2(void) {
  ASSERT_EQ(2, val);
  val = 3;
}

static void handler3(void) {
  ASSERT_EQ(3, val);
  _Exit(86);
}

TEST(at_quick_exit, example) {
  // Fork process.
  int fd;
  int ret = pdfork(&fd);

  // Terminate the child process.
  if (ret == 0) {
    ASSERT_EQ(0, at_quick_exit(handler3));
    ASSERT_EQ(0, at_quick_exit(handler2));
    ASSERT_EQ(0, at_quick_exit(handler1));
    quick_exit(43);
  }

  ASSERT_LT(0, ret);
  siginfo_t si;
  ASSERT_EQ(0, pdwait(fd, &si, 0));
  ASSERT_EQ(0, close(fd));

  // Validate exit code.
  ASSERT_EQ(SIGCHLD, si.si_signo);
  ASSERT_EQ(CLD_EXITED, si.si_code);
  ASSERT_EQ(86, si.si_status);
}
#endif
