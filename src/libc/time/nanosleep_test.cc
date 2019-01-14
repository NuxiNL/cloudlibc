// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <time.h>

#include "gtest/gtest.h"

TEST(nanosleep, bad) {
  // Invalid time value.
  struct timespec ts = {.tv_nsec = -7};
  ASSERT_EQ(-1, nanosleep(&ts));
  ASSERT_EQ(EINVAL, errno);
}

TEST(nanosleep, example) {
  // We should sleep at least 1 second.
  time_t before = time(NULL);
  struct timespec ts = {.tv_sec = 1, .tv_nsec = 500000000L};
  ASSERT_EQ(0, nanosleep(&ts, NULL));
  time_t after = time(NULL);
  ASSERT_LE(before + 1, after);
}
