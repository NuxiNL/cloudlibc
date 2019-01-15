// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <signal.h>
#include <string.h>

#include "gtest/gtest.h"

TEST(strsignal, examples) {
#define TEST_SIGNAL(code, str)          \
  do {                                  \
    errno = 1234;                       \
    ASSERT_STREQ(str, strsignal(code)); \
    ASSERT_EQ(1234, errno);             \
  } while (0)
  TEST_SIGNAL(-1, "Unknown signal");
  TEST_SIGNAL(0, "Unknown signal");

  TEST_SIGNAL(SIGABRT, "Abort trap");
  TEST_SIGNAL(SIGALRM, "Alarm clock");
  TEST_SIGNAL(SIGBUS, "Bus error");
  TEST_SIGNAL(SIGCHLD, "Child exited");
  TEST_SIGNAL(SIGCONT, "Continued");
  TEST_SIGNAL(SIGFPE, "Floating point exception");
  TEST_SIGNAL(SIGHUP, "Hangup");
  TEST_SIGNAL(SIGILL, "Illegal instruction");
  TEST_SIGNAL(SIGINT, "Interrupt");
  TEST_SIGNAL(SIGKILL, "Killed");
  TEST_SIGNAL(SIGPIPE, "Broken pipe");
  TEST_SIGNAL(SIGQUIT, "Quit");
  TEST_SIGNAL(SIGSEGV, "Segmentation fault");
  TEST_SIGNAL(SIGSTOP, "Suspended (signal)");
  TEST_SIGNAL(SIGSYS, "Bad system call");
  TEST_SIGNAL(SIGTERM, "Terminated");
  TEST_SIGNAL(SIGTRAP, "Trace/BPT trap");
  TEST_SIGNAL(SIGTSTP, "Suspended");
  TEST_SIGNAL(SIGTTIN, "Stopped (tty input)");
  TEST_SIGNAL(SIGTTOU, "Stopped (tty output)");
  TEST_SIGNAL(SIGURG, "Urgent I/O condition");
  TEST_SIGNAL(SIGUSR1, "User defined signal 1");
  TEST_SIGNAL(SIGUSR2, "User defined signal 2");
  TEST_SIGNAL(SIGVTALRM, "Virtual timer expired");
  TEST_SIGNAL(SIGXCPU, "Cputime limit exceeded");
  TEST_SIGNAL(SIGXFSZ, "Filesize limit exceeded");
}
