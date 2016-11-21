// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <testing.h>
#include <time.h>

TEST(clock_gettime, monotonic) {
  struct timespec ts;
  ASSERT_EQ(0, clock_gettime(CLOCK_MONOTONIC, &ts));
  ASSERT_LE(0, ts.tv_nsec);
  ASSERT_GT(1000000000, ts.tv_nsec);
}

TEST(clock_gettime, realtime) {
  struct timespec ts;
  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
  ASSERT_LE(1416312511, ts.tv_sec);
  ASSERT_GE((time_t)1416312511 + 30 * 365 * 24 * 60 * 60, ts.tv_sec);
  ASSERT_LE(0, ts.tv_nsec);
  ASSERT_GT(1000000000, ts.tv_nsec);
}
