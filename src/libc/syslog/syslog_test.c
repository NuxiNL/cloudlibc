// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/procdesc.h>

#include <ctype.h>
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

  // Priority has been masked.
  {
    syslog(LOG_CRIT, "Should be discarded");
    fflush(stderr);
    ASSERT_EQ(-1, read(fds[0], NULL, 1));
    ASSERT_EQ(EAGAIN, errno);
  }

#define TEST_EXAMPLE(out, format, ...)                                 \
  do {                                                                 \
    syslog(LOG_DEBUG, format, ##__VA_ARGS__);                          \
    fflush(stderr);                                                    \
    char buf[sizeof(out) + 42];                                        \
    ASSERT_EQ(sizeof(out) + 41, read(fds[0], buf, sizeof(buf)));       \
    /* ISO 8601 timestamp. */                                          \
    ASSERT_EQ('2', buf[0]);                                            \
    ASSERT_EQ('0', buf[1]);                                            \
    ASSERT_TRUE(isdigit(buf[2]));                                      \
    ASSERT_TRUE(isdigit(buf[3]));                                      \
    ASSERT_EQ('-', buf[4]);                                            \
    ASSERT_TRUE(isdigit(buf[5]));                                      \
    ASSERT_TRUE(isdigit(buf[6]));                                      \
    ASSERT_EQ('-', buf[7]);                                            \
    ASSERT_TRUE(isdigit(buf[8]));                                      \
    ASSERT_TRUE(isdigit(buf[9]));                                      \
    ASSERT_EQ('T', buf[10]);                                           \
    ASSERT_TRUE(isdigit(buf[11]));                                     \
    ASSERT_TRUE(isdigit(buf[12]));                                     \
    ASSERT_EQ(':', buf[13]);                                           \
    ASSERT_TRUE(isdigit(buf[14]));                                     \
    ASSERT_TRUE(isdigit(buf[15]));                                     \
    ASSERT_EQ(':', buf[16]);                                           \
    ASSERT_TRUE(isdigit(buf[17]));                                     \
    ASSERT_TRUE(isdigit(buf[18]));                                     \
    ASSERT_EQ('.', buf[19]);                                           \
    ASSERT_TRUE(isdigit(buf[20]));                                     \
    ASSERT_TRUE(isdigit(buf[21]));                                     \
    ASSERT_TRUE(isdigit(buf[22]));                                     \
    ASSERT_TRUE(isdigit(buf[23]));                                     \
    ASSERT_TRUE(isdigit(buf[24]));                                     \
    ASSERT_TRUE(isdigit(buf[25]));                                     \
    ASSERT_TRUE(isdigit(buf[26]));                                     \
    ASSERT_TRUE(isdigit(buf[27]));                                     \
    ASSERT_TRUE(isdigit(buf[28]));                                     \
    /* Severity, followed by message. */                               \
    ASSERT_ARREQ("Z DEBUG     " out "\n", buf + 29, 12 + sizeof(out)); \
  } while (0)
  TEST_EXAMPLE("Dropped 123 packets", "Dropped %d packets", 123);
  TEST_EXAMPLE("Dropped 123 packets 10 seconds ago",
               "Dropped %1$d packets %2$d seconds ago\n", 123, 10);
  TEST_EXAMPLE("Dropped 123 packets 10 seconds ago",
               "Dropped %2$d packets %1$d seconds ago", 10, 123);
  // TODO(edje): Add support for %m and add tests.
#undef TEST_EXAMPLE
}
