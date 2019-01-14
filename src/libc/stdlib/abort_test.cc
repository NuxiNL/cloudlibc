// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "gtest/gtest.h"

#if 0  // TODO(ed): How to test this without forking?
TEST(abort, example) {
  // Fork process.
  int fd;
  int ret = pdfork(&fd);

  // Terminate the child process.
  if (ret == 0)
    abort();

  ASSERT_LT(0, ret);
  siginfo_t si;
  ASSERT_EQ(0, pdwait(fd, &si, 0));
  ASSERT_EQ(0, close(fd));

  // Validate exit code.
  ASSERT_EQ(SIGCHLD, si.si_signo);
  ASSERT_EQ(CLD_KILLED, si.si_code);
  ASSERT_EQ(SIGABRT, si.si_status);
}
#endif
