// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wchar.h>

TEST(wcslcat, null) {
  ASSERT_EQ(5, wcslcat(NULL, L"Hello", 0));
}

TEST(wcslcat, one) {
  wchar_t buf = L'\0';
  ASSERT_EQ(6, wcslcat(&buf, L"Banana", 1));
  ASSERT_EQ(L'\0', buf);

  buf = L'A';
  ASSERT_EQ(7, wcslcat(&buf, L"Banana", 1));
  ASSERT_EQ(L'A', buf);
}

TEST(wcslcat, longer) {
  wchar_t buf[12] = L"AAAAAAAAAAAA";
  ASSERT_EQ(15, wcslcat(buf, L"Foo", __arraycount(buf)));
  ASSERT_ARREQ(L"AAAAAAAAAAAA", buf, __arraycount(buf));

  buf[4] = L'\0';
  ASSERT_EQ(7, wcslcat(buf, L"Bar", __arraycount(buf)));
  ASSERT_ARREQ(L"AAAABar\0AAAA", buf, __arraycount(buf));

  ASSERT_EQ(16, wcslcat(buf, L"Very long", __arraycount(buf)));
  ASSERT_ARREQ(L"AAAABarVery\0", buf, __arraycount(buf));
}
