// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <signal.h>

#include "gtest/gtest.h"

TEST(sigfillset, example) {
  sigset_t set;
  ASSERT_EQ(0, sigfillset(&set));

  // The filled set shall contain all of the standard signals.
  ASSERT_EQ(1, sigismember(&set, SIGABRT));
  ASSERT_EQ(1, sigismember(&set, SIGALRM));
  ASSERT_EQ(1, sigismember(&set, SIGBUS));
  ASSERT_EQ(1, sigismember(&set, SIGCHLD));
  ASSERT_EQ(1, sigismember(&set, SIGCONT));
  ASSERT_EQ(1, sigismember(&set, SIGFPE));
  ASSERT_EQ(1, sigismember(&set, SIGHUP));
  ASSERT_EQ(1, sigismember(&set, SIGILL));
  ASSERT_EQ(1, sigismember(&set, SIGINT));
  ASSERT_EQ(1, sigismember(&set, SIGKILL));
  ASSERT_EQ(1, sigismember(&set, SIGPIPE));
  ASSERT_EQ(1, sigismember(&set, SIGQUIT));
  ASSERT_EQ(1, sigismember(&set, SIGSEGV));
  ASSERT_EQ(1, sigismember(&set, SIGSTOP));
  ASSERT_EQ(1, sigismember(&set, SIGSYS));
  ASSERT_EQ(1, sigismember(&set, SIGTERM));
  ASSERT_EQ(1, sigismember(&set, SIGTRAP));
  ASSERT_EQ(1, sigismember(&set, SIGTSTP));
  ASSERT_EQ(1, sigismember(&set, SIGTTIN));
  ASSERT_EQ(1, sigismember(&set, SIGTTOU));
  ASSERT_EQ(1, sigismember(&set, SIGURG));
  ASSERT_EQ(1, sigismember(&set, SIGUSR1));
  ASSERT_EQ(1, sigismember(&set, SIGUSR2));
  ASSERT_EQ(1, sigismember(&set, SIGVTALRM));
  ASSERT_EQ(1, sigismember(&set, SIGXCPU));
  ASSERT_EQ(1, sigismember(&set, SIGXFSZ));
}
