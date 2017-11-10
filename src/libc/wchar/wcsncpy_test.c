// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <testing.h>
#include <wchar.h>

TEST(wcsncpy, null) {
  ASSERT_EQ((wchar_t *)12, wcsncpy((wchar_t *)12, (wchar_t *)500, 0));
}

TEST(wcsncpy, example1) {
  wchar_t buf[12] = L"AAAAAAAAAAAA";
  ASSERT_EQ(buf, wcsncpy(buf, L"Hello", 12));
  ASSERT_ARREQ(L"Hello\0\0\0\0\0\0\0", buf, 12);
}

TEST(wcsncpy, example2) {
  wchar_t buf[12];
  ASSERT_EQ(buf, wcsncpy(buf, L"This is a very long string", 12));
  ASSERT_ARREQ(L"This is a ve", buf, 12);
}
