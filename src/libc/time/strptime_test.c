// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <locale.h>
#include <testing.h>
#include <time.h>

TEST(strptime, example1) {
  const char input[] = "2001-11-12 18:31:01";
  struct tm tm;
  ASSERT_EQ(input + __arraycount(input) - 1,
            strptime(input, "%Y-%m-%d %H:%M:%S", &tm));
  ASSERT_EQ(101, tm.tm_year);
  ASSERT_EQ(10, tm.tm_mon);
  ASSERT_EQ(12, tm.tm_mday);
  ASSERT_EQ(18, tm.tm_hour);
  ASSERT_EQ(31, tm.tm_min);
  ASSERT_EQ(1, tm.tm_sec);
}

TEST(strptime, example2) {
  const char input[] = "6 Dec 2001 12:33:45";
  struct tm tm;
  ASSERT_EQ(input + __arraycount(input) - 1,
            strptime(input, "%d %b %Y %H:%M:%S", &tm));
  ASSERT_EQ(101, tm.tm_year);
  ASSERT_EQ(11, tm.tm_mon);
  ASSERT_EQ(6, tm.tm_mday);
  ASSERT_EQ(12, tm.tm_hour);
  ASSERT_EQ(33, tm.tm_min);
  ASSERT_EQ(45, tm.tm_sec);
}

TEST(strptime, example3) {
  const char input[] = "19810405T134730.04827+0230";
  struct tm tm;
  ASSERT_EQ(input + __arraycount(input) - 1,
            strptime(input, "%Y%m%dT%H%M%S.%f%z", &tm));
  ASSERT_EQ(81, tm.tm_year);
  ASSERT_EQ(3, tm.tm_mon);
  ASSERT_EQ(5, tm.tm_mday);
  ASSERT_EQ(13, tm.tm_hour);
  ASSERT_EQ(47, tm.tm_min);
  ASSERT_EQ(30, tm.tm_sec);
  ASSERT_EQ(48270000, tm.tm_nsec);
  ASSERT_EQ(9000, tm.tm_gmtoff);
}

TEST(strptime, example4) {
  const char input[] = "7:27:31 pm 10/31/88";
  struct tm tm;
  ASSERT_EQ(input + __arraycount(input) - 1, strptime(input, "%r %D", &tm));
  ASSERT_EQ(88, tm.tm_year);
  ASSERT_EQ(9, tm.tm_mon);
  ASSERT_EQ(31, tm.tm_mday);
  ASSERT_EQ(19, tm.tm_hour);
  ASSERT_EQ(27, tm.tm_min);
  ASSERT_EQ(31, tm.tm_sec);
}

TEST(strptime, bounds) {
  struct tm tm;
  ASSERT_EQ(NULL, strptime("0", "%d", NULL));
  ASSERT_NE(NULL, strptime("1", "%d", &tm));
  ASSERT_EQ(1, tm.tm_mday);
  ASSERT_NE(NULL, strptime("31", "%d", &tm));
  ASSERT_EQ(31, tm.tm_mday);
  ASSERT_NE(NULL, strptime("32", "%d", &tm));
  ASSERT_EQ(3, tm.tm_mday);
}

TEST(strptime, dutch) {
  const char input[] = "woensdag, 31 dec 2014";
  struct tm tm;
  locale_t locale = newlocale(LC_TIME_MASK, "nl_NL", 0);
  ASSERT_EQ(input + __arraycount(input) - 1,
            strptime_l(input, "%a, %d %b %Y", &tm, locale));
  ASSERT_EQ(3, tm.tm_wday);
  ASSERT_EQ(31, tm.tm_mday);
  ASSERT_EQ(11, tm.tm_mon);
  ASSERT_EQ(114, tm.tm_year);
  freelocale(locale);
}
