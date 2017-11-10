// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <testing.h>
#include <wchar.h>

TEST(wmemset, null) {
  ASSERT_EQ((wchar_t *)5, wmemset((wchar_t *)5, L'A', 0));
}

TEST(wmemset, example) {
  wchar_t buf[10];
  ASSERT_EQ(buf, wmemset(buf, L'!', __arraycount(buf)));
  ASSERT_ARREQ(L"!!!!!!!!!!", buf, __arraycount(buf));
}
