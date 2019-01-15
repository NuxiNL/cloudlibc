// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(syslog, example) {
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
  FILE *fp = fdopen(fds[1], "w");
  ASSERT_NE(NULL, fp);
  fswap(stderr, fp);

  // Priority has been masked.
  {
    syslog(LOG_CRIT, "Should be discarded");
    fflush(stderr);
    char b;
    ASSERT_EQ(-1, read(fds[0], &b, 1));
    ASSERT_EQ(EAGAIN, errno);
  }

#define TEST_EXAMPLE(out, format, ...)                                   \
  do {                                                                   \
    syslog(LOG_DEBUG, format, ##__VA_ARGS__);                            \
    fflush(stderr);                                                      \
    char buf[sizeof(out) + 42];                                          \
    ASSERT_EQ(sizeof(out) + 41, read(fds[0], buf, sizeof(buf)));         \
    /* ISO 8601 timestamp. */                                            \
    ASSERT_EQ('2', buf[0]);                                              \
    ASSERT_EQ('0', buf[1]);                                              \
    ASSERT_TRUE(isdigit(buf[2]));                                        \
    ASSERT_TRUE(isdigit(buf[3]));                                        \
    ASSERT_EQ('-', buf[4]);                                              \
    ASSERT_TRUE(isdigit(buf[5]));                                        \
    ASSERT_TRUE(isdigit(buf[6]));                                        \
    ASSERT_EQ('-', buf[7]);                                              \
    ASSERT_TRUE(isdigit(buf[8]));                                        \
    ASSERT_TRUE(isdigit(buf[9]));                                        \
    ASSERT_EQ('T', buf[10]);                                             \
    ASSERT_TRUE(isdigit(buf[11]));                                       \
    ASSERT_TRUE(isdigit(buf[12]));                                       \
    ASSERT_EQ(':', buf[13]);                                             \
    ASSERT_TRUE(isdigit(buf[14]));                                       \
    ASSERT_TRUE(isdigit(buf[15]));                                       \
    ASSERT_EQ(':', buf[16]);                                             \
    ASSERT_TRUE(isdigit(buf[17]));                                       \
    ASSERT_TRUE(isdigit(buf[18]));                                       \
    ASSERT_EQ('.', buf[19]);                                             \
    ASSERT_TRUE(isdigit(buf[20]));                                       \
    ASSERT_TRUE(isdigit(buf[21]));                                       \
    ASSERT_TRUE(isdigit(buf[22]));                                       \
    ASSERT_TRUE(isdigit(buf[23]));                                       \
    ASSERT_TRUE(isdigit(buf[24]));                                       \
    ASSERT_TRUE(isdigit(buf[25]));                                       \
    ASSERT_TRUE(isdigit(buf[26]));                                       \
    ASSERT_TRUE(isdigit(buf[27]));                                       \
    ASSERT_TRUE(isdigit(buf[28]));                                       \
    /* Severity, followed by message. */                                 \
    ASSERT_THAT(buf + 29, testing::StartsWith("Z DEBUG     " out "\n")); \
  } while (0)
  TEST_EXAMPLE("Dropped 123 packets 10 seconds ago",
               "Dropped %d packets %d seconds ago", 123, 10);
  TEST_EXAMPLE("Dropped 123 packets 10 seconds ago",
               "Dropped %1$d packets %2$d seconds ago\n", 123, 10);
  TEST_EXAMPLE("Dropped 123 packets 10 seconds ago",
               "Dropped %2$d packets %1$d seconds ago", 10, 123);

  errno = ENOENT;
  TEST_EXAMPLE(
      "Failed to open file /etc/passwd for reading: No such file or directory",
      "Failed to open file %s for %s: %m", "/etc/passwd", "reading");
  errno = ENOENT;
  TEST_EXAMPLE(
      "Failed to open file /etc/passwd for reading: No such file or directory",
      "Failed to open file %1$s for %2$s: %m", "/etc/passwd", "reading");
  errno = ENOENT;
  TEST_EXAMPLE(
      "Failed to open file /etc/passwd for reading: No such file or directory",
      "Failed to open file %2$s for %1$s: %m", "reading", "/etc/passwd");
#undef TEST_EXAMPLE

  ASSERT_EQ(0, close(fds[0]));
  fswap(stderr, fp);
  ASSERT_EQ(0, fclose(fp));
}
