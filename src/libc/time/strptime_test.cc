// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "gtest/gtest.h"

TEST(strptime, examples) {
#define TEST_STRPTIME(in, fmt, year, mon, wday, mday, hour, min, sec, nsec, \
                      gmtoff)                                               \
  do {                                                                      \
    const char input[] = in;                                                \
    struct tm tm;                                                           \
    ASSERT_EQ(input + sizeof(input) - 1, strptime(input, fmt, &tm));        \
    ASSERT_EQ(year - 1900, tm.tm_year);                                     \
    ASSERT_EQ(mon - 1, tm.tm_mon);                                          \
    ASSERT_EQ(wday, tm.tm_wday);                                            \
    ASSERT_EQ(mday, tm.tm_mday);                                            \
    ASSERT_EQ(hour, tm.tm_hour);                                            \
    ASSERT_EQ(min, tm.tm_min);                                              \
    ASSERT_EQ(sec, tm.tm_sec);                                              \
    ASSERT_EQ(nsec, tm.tm_nsec);                                            \
    ASSERT_EQ(gmtoff, tm.tm_gmtoff);                                        \
    ASSERT_EQ(-1, tm.tm_isdst);                                             \
    ASSERT_EQ(NULL, tm.tm_zone);                                            \
  } while (0)
  // The default date is January 1st, 2000.
  TEST_STRPTIME("", "", 2000, 1, 6, 1, 0, 0, 0, 0, 0);

  TEST_STRPTIME("2001-11-12 18:31:01", "%Y-%m-%d %H:%M:%S", 2001, 11, 1, 12, 18,
                31, 1, 0, 0);
  TEST_STRPTIME("6 Dec 2001 12:33:45", "%d %b %Y %H:%M:%S", 2001, 12, 4, 6, 12,
                33, 45, 0, 0);
  TEST_STRPTIME("19810405T134730.04827+0230", "%Y%m%dT%H%M%S.%f%z", 1981, 4, 0,
                5, 13, 47, 30, 48270000, 9000);
  TEST_STRPTIME("7:27:31 pm 10/31/88", "%r %D", 1988, 10, 1, 31, 19, 27, 31, 0,
                0);

  // Providing the day of the year instead of the actual date.
  TEST_STRPTIME("1998 184", "%Y %j", 1998, 7, 5, 3, 0, 0, 0, 0, 0);

  // ISO 8601 week numbers.
  TEST_STRPTIME("2016-W29", "%G-W%V", 2016, 7, 1, 18, 0, 0, 0, 0, 0);
  TEST_STRPTIME("2016-W29-2", "%G-W%V-%u", 2016, 7, 2, 19, 0, 0, 0, 0, 0);

  // Just weekday or month names.
  TEST_STRPTIME("wednesday", "%a", 2000, 1, 3, 5, 0, 0, 0, 0, 0);
  TEST_STRPTIME("APRIL", "%b", 2000, 4, 6, 1, 0, 0, 0, 0, 0);

  // Corner case: week-based year, week number and day of the month,
  // where the date lies in the previous year.
  TEST_STRPTIME("1902-01-31 12:35:27", "%G-%V-%d %T", 1901, 12, 2, 31, 12, 35,
                27, 0, 0);

  // The century that's used implicitly depends on the last two digits
  // of the year.
  TEST_STRPTIME("-69", "%y", 1969, 1, 3, 1, 0, 0, 0, 0, 0);
  TEST_STRPTIME("+68", "%y", 2068, 1, 0, 1, 0, 0, 0, 0, 0);
#undef TEST_STRPTIME
}

TEST(strptime, random) {
#define TEST_RANDOM(fmt)                                    \
  do {                                                      \
    for (int i = 0; i < 100; ++i) {                         \
      /* Pick a random timestamp. */                        \
      int32_t r;                                            \
      arc4random_buf(&r, sizeof(r));                        \
      time_t t = r;                                         \
      SCOPED_TRACE(t);                                      \
      /* Convert it to a string representation. */          \
      struct tm tm_in;                                      \
      ASSERT_EQ(&tm_in, gmtime_r(&t, &tm_in));              \
      char str[64];                                         \
                                                            \
      /* Parse it back. */                                  \
      size_t len = strftime(str, sizeof(str), fmt, &tm_in); \
      ASSERT_LT(0, len);                                    \
      struct tm tm_out;                                     \
      ASSERT_EQ(str + len, strptime(str, fmt, &tm_out));    \
                                                            \
      /* Test for equality. */                              \
      ASSERT_EQ(tm_in.tm_year, tm_out.tm_year);             \
      ASSERT_EQ(tm_in.tm_yday, tm_out.tm_yday);             \
      ASSERT_EQ(tm_in.tm_mon, tm_out.tm_mon);               \
      ASSERT_EQ(tm_in.tm_mday, tm_out.tm_mday);             \
      ASSERT_EQ(tm_in.tm_wday, tm_out.tm_wday);             \
      ASSERT_EQ(tm_in.tm_hour, tm_out.tm_hour);             \
      ASSERT_EQ(tm_in.tm_min, tm_out.tm_min);               \
      ASSERT_EQ(tm_in.tm_sec, tm_out.tm_sec);               \
      ASSERT_EQ(-1, tm_out.tm_isdst);                       \
      ASSERT_EQ(0, tm_out.tm_gmtoff);                       \
      ASSERT_EQ(NULL, tm_out.tm_zone);                      \
      ASSERT_EQ(0, tm_out.tm_nsec);                         \
    }                                                       \
  } while (0)
  // Year, month, day of the month.
  TEST_RANDOM("%Y-%b-%d %H:%M:%S");
  TEST_RANDOM("%Y-%B-%d %I:%M:%S %p");
  TEST_RANDOM("%Y-%h-%d %T");
  TEST_RANDOM("%Y-%m-%d %T");
  TEST_RANDOM("%F %T");

  // Year, day of the year.
  TEST_RANDOM("%Y-%j %T");

  // Year, week (first Sunday), day of the week.
  TEST_RANDOM("%Y-%U-%a %T");
  TEST_RANDOM("%Y-%U-%A %T");
  TEST_RANDOM("%Y-%U-%u %T");
  TEST_RANDOM("%Y-%U-%w %T");

  // Year, week (first Monday), day of the week.
  TEST_RANDOM("%Y-%W-%a %T");
  TEST_RANDOM("%Y-%W-%A %T");
  TEST_RANDOM("%Y-%W-%u %T");
  TEST_RANDOM("%Y-%W-%w %T");

  // Week-based year, week, day of the week.
  TEST_RANDOM("%G-%V-%a %T");
  TEST_RANDOM("%G-%V-%A %T");
  TEST_RANDOM("%G-%V-%u %T");
  TEST_RANDOM("%G-%V-%w %T");

  // Not common, but not ambiguous: Year, week, day of the month.
  TEST_RANDOM("%Y-%U-%d %T");
  TEST_RANDOM("%Y-%W-%d %T");
  TEST_RANDOM("%G-%V-%d %T");

  // Overlap: prefer fully specified data (mday/mon, wday/week) over
  // partial data (just mday, mon, wday or week).
  TEST_RANDOM("%Y-%b-%j %T");
  TEST_RANDOM("%Y-%j-%b %T");
  TEST_RANDOM("%Y-%W-%j %T");
  TEST_RANDOM("%Y-%j-%W %T");
  TEST_RANDOM("%Y-%b-%d-%W %T");
  TEST_RANDOM("%Y-%W-%b-%d %T");
#undef TEST_RANDOM
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
  ASSERT_EQ(input + sizeof(input) - 1,
            strptime_l(input, "%a, %d %b %Y", &tm, locale));
  ASSERT_EQ(3, tm.tm_wday);
  ASSERT_EQ(31, tm.tm_mday);
  ASSERT_EQ(11, tm.tm_mon);
  ASSERT_EQ(114, tm.tm_year);
  freelocale(locale);
}
