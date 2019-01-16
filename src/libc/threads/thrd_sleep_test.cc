// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <threads.h>

#include "gtest/gtest.h"

TEST(thrd_sleep, bad) {
  // Invalid time value.
  struct timespec ts = {.tv_nsec = -7};
  EXPECT_EQ(-1, thrd_sleep(&ts));
}

TEST(thrd_sleep, example) {
  // We should sleep at least 1 second.
  time_t before = time(NULL);
  struct timespec ts = {.tv_sec = 1, .tv_nsec = 500000000L};
  ASSERT_EQ(0, thrd_sleep(&ts, NULL));
  time_t after = time(NULL);
  ASSERT_LE(before + 1, after);
}
