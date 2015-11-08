// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wchar.h>
#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS
#include <wchar.h>

TEST(wcscpy, example) {
  wchar_t buf[10] = L"AAAAAAAAAA";
  ASSERT_EQ(buf, wcscpy(buf, L""));
  ASSERT_ARREQ(L"\0AAAAAAAAA", buf, __arraycount(buf));
  ASSERT_EQ(buf, wcscpy(buf, L"Hello"));
  ASSERT_ARREQ(L"Hello\0AAAA", buf, __arraycount(buf));
  ASSERT_EQ(buf, wcscpy(buf, L""));
  ASSERT_ARREQ(L"\0ello\0AAAA", buf, __arraycount(buf));
  ASSERT_EQ(buf, wcscpy(buf, L"Example!!"));
  ASSERT_ARREQ(L"Example!!\0", buf, __arraycount(buf));
}
