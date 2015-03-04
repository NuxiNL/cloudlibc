// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wchar.h>

TEST(wcslcpy, null) {
  ASSERT_EQ(5, wcslcpy(NULL, L"Hello", 0));
}

TEST(wcslcpy, one) {
  wchar_t buf;
  ASSERT_EQ(6, wcslcpy(&buf, L"Banana", 1));
  ASSERT_EQ(L'\0', buf);
}

TEST(wcslcpy, longer) {
  wchar_t buf[] = L"AAAAAAAAAA";
  ASSERT_EQ(3, wcslcpy(buf, L"Dog", __arraycount(buf)));
  ASSERT_ARREQ(L"Dog\0AAAAAA", buf, __arraycount(buf));
}

TEST(wcslcpy, longest) {
  wchar_t buf[12];
  ASSERT_EQ(23, wcslcpy(buf, L"This is a long sentence", __arraycount(buf)));
  ASSERT_STREQ(L"This is a l", buf);
}
