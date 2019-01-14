// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <time.h>

#include "gtest/gtest.h"

TEST(ctime_r, examples) {
  char buf[26];
  time_t t;

  // Reference date.
  t = 1441545542;
  ASSERT_EQ(buf, ctime_r(&t, buf));
  ASSERT_STREQ("Sun Sep  6 13:19:02 2015\n", buf);

  // Highest timestamp value supported.
  t = 253402300799;
  ASSERT_EQ(buf, ctime_r(&t, buf));
  ASSERT_STREQ("Fri Dec 31 23:59:59 9999\n", buf);
  t = 253402300800;
  ASSERT_EQ(NULL, ctime_r(&t, buf));
}
