// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>
#include <iterator>

#include "gtest/gtest.h"

TEST(strverscmp, order) {
  const char *strings[] = {
      "",

      "000",
      "00",
      "01",
      "010",
      "09",
      "0",
      "1",
      "2.6.20",
      "2.6.21",
      "9",
      "10",

      // From https://sourceware.org/bugzilla/show_bug.cgi?id=9913.
      "B0075022800016.gbp.corp.com",
      "B007502280067.gbp.corp.com",
      "B007502357019.GBP.CORP.COM",
  };
  for (size_t i = 0; i < std::size(strings); ++i) {
    for (size_t j = 0; j < std::size(strings); ++j) {
      SCOPED_TRACE(i);
      SCOPED_TRACE(j);
      if (i < j)
        ASSERT_GT(0, strverscmp(strings[i], strings[j]));
      else if (i > j)
        ASSERT_LT(0, strverscmp(strings[i], strings[j]));
      else
        ASSERT_EQ(0, strverscmp(strings[i], strings[j]));
    }
  }
}
