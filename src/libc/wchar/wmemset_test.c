// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

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
