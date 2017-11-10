// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <testing.h>
#include <wchar.h>

TEST(wmemcpy, null) {
  ASSERT_EQ((wchar_t *)42, wmemcpy((wchar_t *)42, (wchar_t *)123, 0));
}

TEST(wmemcpy, example) {
  const wchar_t buf1[8] = L"Foo\0Bar";
  wchar_t buf2[8];
  ASSERT_EQ(buf2, wmemcpy(buf2, buf1, __arraycount(buf1)));
  ASSERT_ARREQ(buf1, buf2, __arraycount(buf1));
}
