// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wchar.h>

TEST(wcspbrk, example) {
  const wchar_t *wcs = L"Hello, world";
  ASSERT_EQ(wcs, wcspbrk(wcs, L"H"));
  ASSERT_EQ(wcs + 7, wcspbrk(wcs, L"rdw"));
  ASSERT_EQ(NULL, wcspbrk(wcs, L"XYZ"));
}
