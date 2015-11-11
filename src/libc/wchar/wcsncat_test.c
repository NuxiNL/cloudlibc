// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <testing.h>
#include <wchar.h>

TEST(wcsncat, example) {
  wchar_t buf[10] = L"\0AAAAAAAAA";
  ASSERT_EQ(buf, wcsncat(buf, L"", 0));
  ASSERT_ARREQ(L"\0AAAAAAAAA", buf, __arraycount(buf));
  ASSERT_EQ(buf, wcsncat(buf, L"Hello", 99999));
  ASSERT_ARREQ(L"Hello\0AAAA", buf, __arraycount(buf));
  ASSERT_EQ(buf, wcsncat(buf, L"", 1));
  ASSERT_ARREQ(L"Hello\0AAAA", buf, __arraycount(buf));
  ASSERT_EQ(buf, wcsncat(buf, L"!!!!!!!!!!!!", 3));
  ASSERT_ARREQ(L"Hello!!!\0A", buf, __arraycount(buf));
}
