// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <testing.h>
#include <threads.h>

TEST(thrd_sleep, bad) {
  // Invalid time value.
  ASSERT_EQ(-1, thrd_sleep(&(struct timespec){.tv_nsec = -7}));
}

TEST(thrd_sleep, example) {
  // We should sleep somewhere between 1 and 2 seconds.
  time_t before = time(NULL);
  ASSERT_EQ(
      0,
      thrd_sleep(&(struct timespec){.tv_sec = 1, .tv_nsec = 500000000L}, NULL));
  time_t after = time(NULL);
  ASSERT_TRUE(after - before >= 1 && after - before <= 2);
}
