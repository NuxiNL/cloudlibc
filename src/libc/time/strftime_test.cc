// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <time.h>

#include "gtest/gtest.h"

TEST(strftime, null) {
  ASSERT_EQ(0, strftime(NULL, 0, "Foo", NULL));
}

TEST(strftime, hello_world) {
  char buf[13];
  ASSERT_EQ(12, strftime(buf, sizeof(buf), "Hello, world", NULL));
  ASSERT_STREQ("Hello, world", buf);
}

TEST(strftime, iso8601) {
  struct tm tm = {
      .tm_year = 114,
      .tm_mon = 8,
      .tm_mday = 30,
      .tm_hour = 10,
      .tm_min = 58,
      .tm_sec = 13,
      .tm_nsec = 12345600,
  };
  char buf[33];
  ASSERT_EQ(32, strftime(buf, sizeof(buf), "%FT%T.%f%z", &tm));
  ASSERT_STREQ("2014-09-30T10:58:13.0123456+0000", buf);
}

TEST(strftime, d_t_fmt) {
  struct tm tm = {
      .tm_year = 37,
      .tm_mon = 4,
      .tm_mday = 6,
      .tm_wday = 4,
      .tm_hour = 19,
      .tm_min = 25,
  };
  char buf[25];
  ASSERT_EQ(24, strftime(buf, sizeof(buf), "%c", &tm));
  ASSERT_STREQ("Thu May  6 19:25:00 1937", buf);
}

TEST(strftime, Ffmt) {
  struct tm tm = {
      .tm_year = 115,
      .tm_mon = 2,
      .tm_mday = 6,
  };
  char buf[13];
  ASSERT_EQ(10, strftime(buf, sizeof(buf), "%F", &tm));
  ASSERT_STREQ("2015-03-06", buf);
  ASSERT_EQ(10, strftime(buf, sizeof(buf), "%3F", &tm));
  ASSERT_STREQ("2015-03-06", buf);
  ASSERT_EQ(10, strftime(buf, sizeof(buf), "%9F", &tm));
  ASSERT_STREQ("2015-03-06", buf);
  ASSERT_EQ(10, strftime(buf, sizeof(buf), "%10F", &tm));
  ASSERT_STREQ("2015-03-06", buf);

  ASSERT_EQ(11, strftime(buf, sizeof(buf), "%11F", &tm));
  ASSERT_STREQ(" 2015-03-06", buf);
  ASSERT_EQ(11, strftime(buf, sizeof(buf), "%011F", &tm));
  ASSERT_STREQ("02015-03-06", buf);
  ASSERT_EQ(11, strftime(buf, sizeof(buf), "%+011F", &tm));
  ASSERT_STREQ("+2015-03-06", buf);

  ASSERT_EQ(12, strftime(buf, sizeof(buf), "%12F", &tm));
  ASSERT_STREQ("  2015-03-06", buf);
  ASSERT_EQ(12, strftime(buf, sizeof(buf), "%012F", &tm));
  ASSERT_STREQ("002015-03-06", buf);
  ASSERT_EQ(12, strftime(buf, sizeof(buf), "%+012F", &tm));
  ASSERT_STREQ("+02015-03-06", buf);
}
