// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <time.h>

#include "gtest/gtest.h"

TEST(localtime_r, recent) {
  // 2014-11-20T14:37:31Z. As there is no system-wide timezone available
  // to use, localtime_r() should just return the time in UTC.
  time_t timestamp = 1416494251;
  struct tm tm;
  ASSERT_EQ(&tm, localtime_r(&timestamp, &tm));
  ASSERT_EQ(31, tm.tm_sec);
  ASSERT_EQ(37, tm.tm_min);
  ASSERT_EQ(14, tm.tm_hour);
  ASSERT_EQ(20, tm.tm_mday);
  ASSERT_EQ(10, tm.tm_mon);
  ASSERT_EQ(114, tm.tm_year);
  ASSERT_EQ(4, tm.tm_wday);
  ASSERT_EQ(323, tm.tm_yday);
  ASSERT_EQ(0, tm.tm_isdst);

  ASSERT_EQ(0, tm.tm_gmtoff);
  ASSERT_STREQ("UTC", tm.tm_zone);
  ASSERT_EQ(0, tm.tm_nsec);

  ASSERT_EQ(timestamp, mktime(&tm));
}
