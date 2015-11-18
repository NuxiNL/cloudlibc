// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wchar.h>

TEST(wcsverscmp, order) {
  const wchar_t *strings[] = {
      L"",

      L"000", L"00", L"01", L"010", L"09", L"0", L"1", L"2.6.20", L"2.6.21",
      L"9", L"10",

      // From https://sourceware.org/bugzilla/show_bug.cgi?id=9913.
      L"B0075022800016.gbp.corp.com", L"B007502280067.gbp.corp.com",
      L"B007502357019.GBP.CORP.COM",
  };
  for (size_t i = 0; i < __arraycount(strings); ++i) {
    for (size_t j = 0; j < __arraycount(strings); ++j) {
      SCOPED_NOTE(i, {
        SCOPED_NOTE(j, {
          if (i < j)
            ASSERT_GT(0, wcsverscmp(strings[i], strings[j]));
          else if (i > j)
            ASSERT_LT(0, wcsverscmp(strings[i], strings[j]));
          else
            ASSERT_EQ(0, wcsverscmp(strings[i], strings[j]));
        });
      });
    }
  }
}
