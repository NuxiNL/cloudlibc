// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <time.h>

#include "gtest/gtest.h"

TEST(time, null) {
  struct timespec ts;
  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
  time_t t = time(NULL);

  // Resulting time should be about the same as CLOCK_REALTIME.
  ASSERT_EQ(1, t / ts.tv_sec);
}

TEST(time, nonnull) {
  time_t t1;
  time_t t2 = time(&t1);
  ASSERT_EQ(t1, t2);
}
