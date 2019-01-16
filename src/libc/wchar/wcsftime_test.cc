// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <time.h>
#include <wchar.h>
#include <iterator>

#include "gtest/gtest.h"

TEST(wcsftime, years) {
  struct tm tm;
  wchar_t buf[16];
#define TEST_YEAR(year, conv, output)                                         \
  tm.tm_year = year - 1900;                                                   \
  ASSERT_EQ(std::size(output) - 1, wcsftime(buf, std::size(buf), conv, &tm)); \
  ASSERT_STREQ(output, buf)
  // Examples taken from the strfmon() rationale.
  TEST_YEAR(1970, L"%Y", L"1970");
  TEST_YEAR(1970, L"%+4Y", L"1970");
  TEST_YEAR(27, L"%Y", L"0027");
  TEST_YEAR(270, L"%Y", L"0270");
  TEST_YEAR(270, L"%+4Y", L"0270");
  TEST_YEAR(17, L"%C%y", L"0017");
  TEST_YEAR(270, L"%C%y", L"0270");
  TEST_YEAR(12345, L"%Y", L"12345");
  TEST_YEAR(12345, L"%+4Y", L"+12345");
  TEST_YEAR(12345, L"%05Y", L"12345");
  TEST_YEAR(270, L"%+5Y", L"+0270");
  TEST_YEAR(270, L"%+3C%y", L"+0270");
  TEST_YEAR(12345, L"%+5Y", L"+12345");
  TEST_YEAR(12345, L"%+3C%y", L"+12345");
  TEST_YEAR(12345, L"%06Y", L"012345");
  TEST_YEAR(12345, L"%04C%y", L"012345");
  TEST_YEAR(12345, L"%+6Y", L"+12345");
  TEST_YEAR(12345, L"%+4C%y", L"+12345");
  TEST_YEAR(123456, L"%08Y", L"00123456");
  TEST_YEAR(123456, L"%06C%y", L"00123456");
  TEST_YEAR(123456, L"%+8Y", L"+0123456");
  TEST_YEAR(123456, L"%+6C%y", L"+0123456");

  // Whitespace padding.
  TEST_YEAR(123456, L"%8Y", L"  123456");
  TEST_YEAR(123456, L"%6C%y", L"  123456");

  // Test negative years as well.
  TEST_YEAR(1423, L"%Y", L"1423");
  TEST_YEAR(1423, L"%C%y", L"1423");
  TEST_YEAR(12, L"%Y", L"0012");
  TEST_YEAR(12, L"%C%y", L"0012");
  TEST_YEAR(-5, L"%Y", L"-0005");
  TEST_YEAR(-5, L"%C%y", L"-0005");
  TEST_YEAR(-1999, L"%Y", L"-1999");
  TEST_YEAR(-1999, L"%C%y", L"-1999");
  TEST_YEAR(-2000, L"%Y", L"-2000");
  TEST_YEAR(-2000, L"%C%y", L"-2000");
  TEST_YEAR(-999999, L"%10Y", L"-000999999");
  TEST_YEAR(-999999, L"%8C%y", L"-000999999");
#undef TEST_YEAR
}

TEST(wcsftime, Ufmt) {
  struct tm tm;
  wchar_t buf[3];
#define TEST_WEEK(yday, wday, output)                                          \
  tm.tm_yday = yday;                                                           \
  tm.tm_wday = wday;                                                           \
  ASSERT_EQ(std::size(output) - 1, wcsftime(buf, std::size(buf), L"%U", &tm)); \
  ASSERT_STREQ(output, buf);
  TEST_WEEK(0, 0, L"01");
  TEST_WEEK(0, 1, L"00");
  TEST_WEEK(0, 2, L"00");
  TEST_WEEK(0, 3, L"00");
  TEST_WEEK(0, 4, L"00");
  TEST_WEEK(0, 5, L"00");
  TEST_WEEK(0, 6, L"00");
  TEST_WEEK(5, 0, L"01");
  TEST_WEEK(5, 1, L"01");
  TEST_WEEK(5, 2, L"01");
  TEST_WEEK(5, 3, L"01");
  TEST_WEEK(5, 4, L"01");
  TEST_WEEK(5, 5, L"01");
  TEST_WEEK(5, 6, L"00");
  TEST_WEEK(9, 0, L"02");
  TEST_WEEK(9, 1, L"02");
  TEST_WEEK(9, 2, L"02");
  TEST_WEEK(9, 3, L"01");
  TEST_WEEK(9, 4, L"01");
  TEST_WEEK(9, 5, L"01");
  TEST_WEEK(9, 6, L"01");
#undef TEST_WEEK
}

TEST(wcsftime, iso8601yearweek) {
  struct tm tm;
  wchar_t buf[11];
#define TEST_YEARWEEK(year, yday, wday, output)               \
  tm.tm_year = year - 1900;                                   \
  tm.tm_yday = yday;                                          \
  tm.tm_wday = wday;                                          \
  ASSERT_EQ(std::size(output) - 1,                            \
            wcsftime(buf, std::size(buf), L"%g %G %V", &tm)); \
  ASSERT_STREQ(output, buf);
  TEST_YEARWEEK(1996, 363, 1, L"97 1997 01");  // Monday, 1996-12-30.
  TEST_YEARWEEK(1997, 4, 0, L"97 1997 01");    // Sunday, 1997-01-05.
  TEST_YEARWEEK(1997, 363, 2, L"98 1998 01");  // Tuesday, 1997-12-30.
  TEST_YEARWEEK(1999, 1, 6, L"98 1998 53");    // Saturday, 1999-01-02.
#undef TEST_YEARWEEK
}
