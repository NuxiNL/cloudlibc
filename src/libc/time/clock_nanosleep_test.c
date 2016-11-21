// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
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
#if 0  // TODO(ed): Restore this once we have pthread_getcpuclockid().
  ASSERT_EQ(ENOTSUP, clock_nanosleep(0xdeadc0de, TIMER_ABSTIME,
                                     &(struct timespec){.tv_sec = 1438708721}));
#endif
}

TEST(clock_nanosleep, monotonic_relative) {
  // We should sleep at least 1 second.
  time_t before = time(NULL);
  ASSERT_EQ(
      0, clock_nanosleep(CLOCK_MONOTONIC, 0,
                         &(struct timespec){.tv_sec = 1, .tv_nsec = 500000000L},
                         NULL));
  time_t after = time(NULL);
  ASSERT_LE(before + 1, after);
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
