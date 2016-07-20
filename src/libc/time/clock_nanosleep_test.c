// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <testing.h>
#include <time.h>

TEST(clock_nanosleep, bad) {
  ASSERT_EQ(EINVAL, clock_nanosleep(CLOCK_REALTIME, 0,
                                    &(struct timespec){.tv_nsec = -7}));
  ASSERT_EQ(EINVAL,
            clock_nanosleep(CLOCK_REALTIME, 0xdeadc0de, &(struct timespec){}));
  ASSERT_EQ(ENOTSUP, clock_nanosleep(0xdeadc0de, TIMER_ABSTIME,
                                     &(struct timespec){.tv_sec = 1438708721}));
}

TEST(clock_nanosleep, monotonic_relative) {
  // We should sleep somewhere between 1 and 3 seconds.
  time_t before = time(NULL);
  ASSERT_EQ(
      0, clock_nanosleep(CLOCK_MONOTONIC, 0,
                         &(struct timespec){.tv_sec = 1, .tv_nsec = 500000000L},
                         NULL));
  time_t after = time(NULL);
  ASSERT_LE(before + 1, after);
  ASSERT_GE(before + 3, after);
}

#if 0  // TODO(ed): Re-enable once FreeBSD's nanosleep works.
TEST(clock_nanosleep, realtime_absolute) {
  // Sleep until the next 1-second boundary.
  time_t end = time(NULL) + 1;
  ASSERT_EQ(0, clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME,
                               &(struct timespec){.tv_sec = end}, NULL));
  ASSERT_EQ(end, time(NULL));
}
#endif
