// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <time.h>

#include "gtest/gtest.h"

TEST(timespec_get, bad) {
  ASSERT_EQ(0, timespec_get(NULL, 0xdeadc0de));
}

TEST(timespec_get, time_utc) {
  struct timespec ts1;
  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts1));
  struct timespec ts2;
  ASSERT_EQ(TIME_UTC, timespec_get(&ts2, TIME_UTC));

  // Resulting time should be about the same as CLOCK_REALTIME.
  ASSERT_EQ(1, ts2.tv_sec / ts1.tv_sec);
  ASSERT_LE(0, ts2.tv_nsec);
  ASSERT_GT(1000000000, ts2.tv_nsec);
}
