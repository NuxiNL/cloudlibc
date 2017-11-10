// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <testing.h>
#include <threads.h>

TEST(thrd_sleep, bad) {
  // Invalid time value.
  ASSERT_EQ(-1, thrd_sleep(&(struct timespec){.tv_nsec = -7}));
}

TEST(thrd_sleep, example) {
  // We should sleep at least 1 second.
  time_t before = time(NULL);
  ASSERT_EQ(
      0,
      thrd_sleep(&(struct timespec){.tv_sec = 1, .tv_nsec = 500000000L}, NULL));
  time_t after = time(NULL);
  ASSERT_LE(before + 1, after);
}
