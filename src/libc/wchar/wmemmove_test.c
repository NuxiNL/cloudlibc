// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wchar.h>

TEST(wmemmove, null) {
  ASSERT_EQ((wchar_t *)42, wmemmove((wchar_t *)42, (wchar_t *)34, 0));
}

TEST(wmemmove, example1) {
  wchar_t buf[12] = L"abcdefghijkl";
  ASSERT_EQ(buf, wmemmove(buf, buf + 4, 8));
  ASSERT_ARREQ(L"efghijklijkl", buf, __arraycount(buf));
}

TEST(wmemmove, example2) {
  wchar_t buf[12] = L"abcdefghijkl";
  ASSERT_EQ(buf + 4, wmemmove(buf + 4, buf, 8));
  ASSERT_ARREQ(L"abcdabcdefgh", buf, __arraycount(buf));
}
