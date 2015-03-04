// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sched.h>
#include <testing.h>

TEST(sched_yield, example) {
  ASSERT_EQ(0, sched_yield());
}
