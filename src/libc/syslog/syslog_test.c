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

TEST_SEPARATE_PROCESS(syslog, example) {
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
}
