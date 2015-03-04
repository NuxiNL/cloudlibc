// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wchar.h>

TEST(wcsncasecmp, null) {
  ASSERT_EQ(0, wcsncasecmp(NULL, NULL, 0));
}

TEST(wcsncasecmp, example) {
  ASSERT_EQ(0, wcsncasecmp(L"hello", L"hello", 100));
  ASSERT_EQ(0, wcsncasecmp(L"hElLo", L"hello", 100));

  ASSERT_EQ(0, wcsncasecmp(L"doge", L"dogS", 3));
  ASSERT_GT(0, wcsncasecmp(L"doge", L"dogS", 4));
  ASSERT_EQ(0, wcsncasecmp(L"dogs", L"dogE", 3));
  ASSERT_LT(0, wcsncasecmp(L"dogs", L"dogE", 4));

  ASSERT_EQ(0, wcsncasecmp(L"München?", L"MÜNCHEN!", 7));
  ASSERT_LT(0, wcsncasecmp(L"München?", L"MÜNCHEN!", 8));
}
