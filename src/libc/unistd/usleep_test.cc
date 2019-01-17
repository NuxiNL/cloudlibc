// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <time.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(usleep, example) {
  // POSIX usleep() only allows threads to sleep up to one second. Most
  // implementations out there don't have this restriction. We'd better
  // remain compatible with the rest.
  time_t before = time(NULL);
  ASSERT_EQ(0, usleep(1500000));
  time_t after = time(NULL);
  ASSERT_LE(before + 1, after);
}
