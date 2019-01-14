// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <time.h>

#include "gtest/gtest.h"

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
