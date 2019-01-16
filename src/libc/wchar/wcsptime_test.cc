// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <time.h>
#include <wchar.h>

#include "gtest/gtest.h"

TEST(wcsptime, years) {
  struct tm tm;
  const wchar_t *buf;
#define TEST_YEAR(input, processed, conv, year)         \
  buf = input;                                          \
  ASSERT_EQ(buf + processed, wcsptime(buf, conv, &tm)); \
  ASSERT_EQ(year - 1900, tm.tm_year)
  // Examples taken from the strfmon() rationale.
  TEST_YEAR(L"19700", 4, L"%Y", 1970);
  TEST_YEAR(L"19700", 4, L"%+4Y", 1970);
  TEST_YEAR(L"27", 2, L"%Y", 27);
  TEST_YEAR(L"027", 3, L"%Y", 27);
  TEST_YEAR(L"0027", 4, L"%Y", 27);
  TEST_YEAR(L"00270", 4, L"%Y", 27);
  TEST_YEAR(L"270", 3, L"%Y", 270);
  TEST_YEAR(L"0270", 4, L"%Y", 270);
  TEST_YEAR(L"02700", 4, L"%Y", 270);
  TEST_YEAR(L"270", 3, L"%Y", 270);
  TEST_YEAR(L"0270", 4, L"%+4Y", 270);
  TEST_YEAR(L"02700", 4, L"%+4Y", 270);
  TEST_YEAR(L"00170", 4, L"%C%y", 17);
  TEST_YEAR(L"02700", 4, L"%C%y", 270);
  TEST_YEAR(L"12345", 4, L"%Y", 1234);
  TEST_YEAR(L"12345", 4, L"%+4Y", 1234);
  TEST_YEAR(L"12345", 5, L"%05Y", 12345);
  TEST_YEAR(L"+0270", 5, L"%+5Y", 270);
  TEST_YEAR(L"+0270", 5, L"%+3C%y", 270);
  TEST_YEAR(L"+12345", 5, L"%+5Y", 1234);
  TEST_YEAR(L"+12345", 5, L"%+3C%y", 1234);
  TEST_YEAR(L"012345", 6, L"%06Y", 12345);
  TEST_YEAR(L"012345", 6, L"%04C%y", 12345);
  TEST_YEAR(L"+12345", 6, L"%+6Y", 12345);
  TEST_YEAR(L"+12345", 6, L"%+4C%y", 12345);

  // Test negative years as well.
  TEST_YEAR(L"1423", 4, L"%Y", 1423);
  TEST_YEAR(L"1423", 4, L"%C%y", 1423);
  TEST_YEAR(L"0012", 4, L"%Y", 12);
  TEST_YEAR(L"0012", 4, L"%C%y", 12);
  TEST_YEAR(L"-005", 4, L"%Y", -5);
  TEST_YEAR(L"-005", 4, L"%C%y", -5);
  TEST_YEAR(L"-1999", 5, L"%5Y", -1999);
  TEST_YEAR(L"-1999", 5, L"%3C%y", -1999);
  TEST_YEAR(L"-2000", 5, L"%12Y", -2000);
  TEST_YEAR(L"-2000", 5, L"%3C%y", -2000);
  TEST_YEAR(L"-000999999", 10, L"%10Y", -999999);
  TEST_YEAR(L"-000999999", 10, L"%8C%y", -999999);
#undef TEST_YEAR
}
