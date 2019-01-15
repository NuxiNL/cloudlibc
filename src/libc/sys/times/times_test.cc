// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/times.h>

#include <time.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(times, example) {
  // Obtain running times of the process. As we don't support a process
  // hierarchy through traditional forking, times for child processes
  // should be zero.
  struct tms tms_before;
  clock_t before = times(&tms_before);
  ASSERT_EQ(0, tms_before.tms_cutime);
  ASSERT_EQ(0, tms_before.tms_cstime);

  // Sleep one second.
  ASSERT_EQ(0, sleep(1));

  // Somewhere between 0.5 and 1.5 seconds should have passed.
  struct tms tms_after;
  clock_t after = times(&tms_after);
  ASSERT_LE(CLOCKS_PER_SEC / 2, after - before);
  ASSERT_GE(3 * CLOCKS_PER_SEC / 2, after - before);
  ASSERT_EQ(0, tms_before.tms_cutime);
  ASSERT_EQ(0, tms_before.tms_cstime);
  ASSERT_LE(tms_before.tms_utime, tms_after.tms_utime);
  ASSERT_LE(tms_before.tms_stime, tms_after.tms_stime);
}
