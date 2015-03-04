// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wchar.h>

TEST(wcscspn, example) {
  const wchar_t *wcs = L"Hello, world";
  ASSERT_EQ(0, wcscspn(wcs, L"H"));
  ASSERT_EQ(7, wcscspn(wcs, L"rdw"));
  ASSERT_EQ(12, wcscspn(wcs, L"XYZ"));
}
