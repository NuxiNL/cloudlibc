// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/procdesc.h>

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <testing.h>
#include <unistd.h>

TEST(syslog, example) {
  int fd;
  int ret = pdfork(&fd);
  if (ret == 0) {
    // Set log mask to a fixed value.
    setlogmask(LOG_MASK(LOG_DEBUG));
    ASSERT_EQ(LOG_MASK(LOG_DEBUG), setlogmask(0));
    ASSERT_EQ(LOG_MASK(LOG_DEBUG),
              setlogmask(LOG_MASK(LOG_DEBUG) | LOG_MASK(LOG_INFO)));
    ASSERT_EQ(LOG_MASK(LOG_DEBUG) | LOG_MASK(LOG_INFO), setlogmask(0));

    // Create a pipe and let stderr point to it.
    int fds[2];
    ASSERT_EQ(0, pipe(fds));
    ASSERT_EQ(0, fcntl(fds[0], F_SETFL, fcntl(fds[0], F_GETFL) | O_NONBLOCK));
    fswap(stderr, fdopen(fds[1], "w"));

    // TODO(edje): Add more tests.
    {
      syslog(LOG_CRIT, "Should be discarded");
      ASSERT_EQ(-1, read(fds[0], NULL, 1));
      ASSERT_EQ(EAGAIN, errno);
    }

    _Exit(123);
  }

  // In the parent process, wait for the child process to die.
  ASSERT_LT(0, ret);
  siginfo_t si;
  ASSERT_EQ(0, pdwait(fd, &si, 0));
  ASSERT_EQ(0, close(fd));

  // Validate return code.
  ASSERT_EQ(SIGCHLD, si.si_signo);
  ASSERT_EQ(CLD_EXITED, si.si_code);
  ASSERT_EQ(123, si.si_status);
}
