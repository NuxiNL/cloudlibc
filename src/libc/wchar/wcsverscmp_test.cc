// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>
#include <iterator>

#include "gtest/gtest.h"

TEST(wcsverscmp, order) {
  const wchar_t *strings[] = {
      L"",

      L"000",
      L"00",
      L"01",
      L"010",
      L"09",
      L"0",
      L"1",
      L"2.6.20",
      L"2.6.21",
      L"9",
      L"10",

      // From https://sourceware.org/bugzilla/show_bug.cgi?id=9913.
      L"B0075022800016.gbp.corp.com",
      L"B007502280067.gbp.corp.com",
      L"B007502357019.GBP.CORP.COM",
  };
  for (size_t i = 0; i < std::size(strings); ++i) {
    for (size_t j = 0; j < std::size(strings); ++j) {
      SCOPED_TRACE(i);
      SCOPED_TRACE(j);
      if (i < j)
        ASSERT_GT(0, wcsverscmp(strings[i], strings[j]));
      else if (i > j)
        ASSERT_LT(0, wcsverscmp(strings[i], strings[j]));
      else
        ASSERT_EQ(0, wcsverscmp(strings[i], strings[j]));
    }
  }
}
