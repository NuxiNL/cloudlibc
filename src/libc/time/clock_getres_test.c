// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <testing.h>
#include <time.h>

TEST(clock_getres, bad) {
  ASSERT_EQ(-1, clock_getres(0xdeadc0de, NULL));
  ASSERT_EQ(EINVAL, errno);
}

TEST(clock_getres, monotonic) {
  struct timespec ts;
  ASSERT_EQ(0, clock_getres(CLOCK_MONOTONIC, &ts));
  ASSERT_EQ(0, ts.tv_sec);
  ASSERT_LE(0, ts.tv_nsec);
  ASSERT_GT(1000000000, ts.tv_nsec);
}

TEST(clock_getres, realtime) {
  struct timespec ts;
  ASSERT_EQ(0, clock_getres(CLOCK_REALTIME, &ts));
  ASSERT_EQ(0, ts.tv_sec);
  ASSERT_LE(0, ts.tv_nsec);
  ASSERT_GT(1000000000, ts.tv_nsec);
}
