// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <unistd.h>
#include <time.h>

#include "gtest/gtest.h"

TEST(sleep, example) {
  // We should sleep at least 1 second.
  time_t before = time(NULL);
  ASSERT_EQ(0, sleep(2));
  time_t after = time(NULL);
  ASSERT_LE(before + 1, after);
}
