// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <time.h>
#include <unistd.h>

TEST(usleep, example) {
  // POSIX usleep() only allows threads to sleep up to one second. Most
  // implementations out there don't have this restriction. We'd better
  // remain compatible with the rest.
  time_t before = time(NULL);
  ASSERT_EQ(0, usleep(1500000));
  time_t after = time(NULL);
  ASSERT_LE(before + 1, after);
  ASSERT_GE(before + 4, after);
}
