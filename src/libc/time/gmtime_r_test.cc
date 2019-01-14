// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <time.h>

#include "gtest/gtest.h"

TEST(gmtime_r, toolow) {
  // Would overflow tm.tm_year below INT_MIN.
  time_t timestamp = ((time_t)INT_MIN - 70) * 31536000 +
                     (((time_t)INT_MIN - 72) / 4) * 86400 -
                     (((time_t)INT_MIN - 100) / 100) * 86400 +
                     (((time_t)INT_MIN - 100) / 400) * 86400 - 1;
  struct tm tm;
  ASSERT_EQ(NULL, gmtime_r(&timestamp, &tm));
  ASSERT_EQ(EOVERFLOW, errno);
}

TEST(gmtime_r, lowest) {
  // -(INT_MIN+100)-01-01T00:00:00Z.
  time_t timestamp = ((time_t)INT_MIN - 70) * 31536000 +
                     (((time_t)INT_MIN - 72) / 4) * 86400 -
                     (((time_t)INT_MIN - 100) / 100) * 86400 +
                     (((time_t)INT_MIN - 100) / 400) * 86400;
  struct tm tm;
  ASSERT_EQ(&tm, gmtime_r(&timestamp, &tm));
  ASSERT_EQ(0, tm.tm_sec);
  ASSERT_EQ(0, tm.tm_min);
  ASSERT_EQ(0, tm.tm_hour);
  ASSERT_EQ(1, tm.tm_mday);
  ASSERT_EQ(0, tm.tm_mon);
  ASSERT_EQ(INT_MIN, tm.tm_year);
  ASSERT_EQ(0, tm.tm_yday);
  ASSERT_EQ(0, tm.tm_isdst);

  ASSERT_EQ(0, tm.tm_gmtoff);
  ASSERT_STREQ("UTC", tm.tm_zone);
  ASSERT_EQ(0, tm.tm_nsec);

  ASSERT_EQ(timestamp, timegm(&tm));
}

TEST(gmtime_r, past) {
  // -588499-06-14T11:30:51Z.
  time_t timestamp = -18573429472149;
  struct tm tm;
  ASSERT_EQ(&tm, gmtime_r(&timestamp, &tm));
  ASSERT_EQ(51, tm.tm_sec);
  ASSERT_EQ(30, tm.tm_min);
  ASSERT_EQ(11, tm.tm_hour);
  ASSERT_EQ(14, tm.tm_mday);
  ASSERT_EQ(5, tm.tm_mon);
  ASSERT_EQ(-588499, tm.tm_year);
  ASSERT_EQ(0, tm.tm_wday);
  ASSERT_EQ(164, tm.tm_yday);
  ASSERT_EQ(0, tm.tm_isdst);
  ASSERT_EQ(timestamp, timegm(&tm));
}

TEST(gmtime_r, just_before_epoch) {
  // 1969-12-31T23:59:59Z.
  time_t timestamp = -1;
  struct tm tm;
  ASSERT_EQ(&tm, gmtime_r(&timestamp, &tm));
  ASSERT_EQ(59, tm.tm_sec);
  ASSERT_EQ(59, tm.tm_min);
  ASSERT_EQ(23, tm.tm_hour);
  ASSERT_EQ(31, tm.tm_mday);
  ASSERT_EQ(11, tm.tm_mon);
  ASSERT_EQ(69, tm.tm_year);
  ASSERT_EQ(3, tm.tm_wday);
  ASSERT_EQ(364, tm.tm_yday);
  ASSERT_EQ(0, tm.tm_isdst);
  ASSERT_EQ(timestamp, timegm(&tm));
}

TEST(gmtime_r, epoch) {
  // 1970-01-01T00:00:00Z.
  time_t timestamp = 0;
  struct tm tm;
  ASSERT_EQ(&tm, gmtime_r(&timestamp, &tm));
  ASSERT_EQ(0, tm.tm_sec);
  ASSERT_EQ(0, tm.tm_min);
  ASSERT_EQ(0, tm.tm_hour);
  ASSERT_EQ(1, tm.tm_mday);
  ASSERT_EQ(0, tm.tm_mon);
  ASSERT_EQ(70, tm.tm_year);
  ASSERT_EQ(4, tm.tm_wday);
  ASSERT_EQ(0, tm.tm_yday);
  ASSERT_EQ(0, tm.tm_isdst);
  ASSERT_EQ(timestamp, timegm(&tm));
}

TEST(gmtime_r, recent) {
  // 2014-09-29T05:59:27Z.
  time_t timestamp = 1411970367;
  struct tm tm;
  ASSERT_EQ(&tm, gmtime_r(&timestamp, &tm));
  ASSERT_EQ(27, tm.tm_sec);
  ASSERT_EQ(59, tm.tm_min);
  ASSERT_EQ(5, tm.tm_hour);
  ASSERT_EQ(29, tm.tm_mday);
  ASSERT_EQ(8, tm.tm_mon);
  ASSERT_EQ(114, tm.tm_year);
  ASSERT_EQ(1, tm.tm_wday);
  ASSERT_EQ(271, tm.tm_yday);
  ASSERT_EQ(0, tm.tm_isdst);
  ASSERT_EQ(timestamp, timegm(&tm));
}

TEST(gmtime_r, future) {
  // +590538-07-20T12:29:09Z.
  time_t timestamp = 18573429472149;
  struct tm tm;
  ASSERT_EQ(&tm, gmtime_r(&timestamp, &tm));
  ASSERT_EQ(9, tm.tm_sec);
  ASSERT_EQ(29, tm.tm_min);
  ASSERT_EQ(12, tm.tm_hour);
  ASSERT_EQ(20, tm.tm_mday);
  ASSERT_EQ(6, tm.tm_mon);
  ASSERT_EQ(588638, tm.tm_year);
  ASSERT_EQ(0, tm.tm_wday);
  ASSERT_EQ(200, tm.tm_yday);
  ASSERT_EQ(0, tm.tm_isdst);
  ASSERT_EQ(timestamp, timegm(&tm));
}

TEST(gmtime_r, highest) {
  // +(INT_MAX+100)-12-31T23:59:59Z.
  time_t timestamp = ((time_t)INT_MAX - 69) * 31536000 +
                     (((time_t)INT_MAX - 68) / 4) * 86400 -
                     ((time_t)INT_MAX / 100) * 86400 +
                     (((time_t)INT_MAX + 300) / 400) * 86400 - 1;
  struct tm tm;
  ASSERT_EQ(&tm, gmtime_r(&timestamp, &tm));
  ASSERT_EQ(59, tm.tm_sec);
  ASSERT_EQ(59, tm.tm_min);
  ASSERT_EQ(23, tm.tm_hour);
  ASSERT_EQ(31, tm.tm_mday);
  ASSERT_EQ(11, tm.tm_mon);
  ASSERT_EQ(INT_MAX, tm.tm_year);
  ASSERT_TRUE(tm.tm_yday == 364 || tm.tm_yday == 365);
  ASSERT_EQ(0, tm.tm_isdst);
  ASSERT_EQ(timestamp, timegm(&tm));
}

TEST(gmtime_r, toohigh) {
  // Would overflow tm.tm_year above INT_MAX.
  time_t timestamp =
      ((time_t)INT_MAX - 69) * 31536000 + (((time_t)INT_MAX - 68) / 4) * 86400 -
      ((time_t)INT_MAX / 100) * 86400 + (((time_t)INT_MAX + 300) / 400) * 86400;
  struct tm tm;
  ASSERT_EQ(NULL, gmtime_r(&timestamp, &tm));
  ASSERT_EQ(EOVERFLOW, errno);
}
