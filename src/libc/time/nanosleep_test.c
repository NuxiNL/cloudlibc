// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <testing.h>
#include <time.h>

TEST(nanosleep, bad) {
  // Invalid time value.
  ASSERT_EQ(-1, nanosleep(&(struct timespec){.tv_nsec = -7}));
  ASSERT_EQ(EINVAL, errno);
}

TEST(nanosleep, example) {
  // We should sleep somewhere between 1 and 2 seconds.
  time_t before = time(NULL);
  ASSERT_EQ(0, nanosleep(&(struct timespec){.tv_sec = 1, .tv_nsec = 500000000L},
                         NULL));
  time_t after = time(NULL);
  ASSERT_LE(before + 1, after);
  ASSERT_GE(before + 2, after);
}
