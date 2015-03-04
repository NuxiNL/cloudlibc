// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <time.h>

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
