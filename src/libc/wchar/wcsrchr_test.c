// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wchar.h>

TEST(wcsrchr, examples) {
  const wchar_t *wcs = L"Hello, world";
  ASSERT_EQ(NULL, wcsrchr(wcs, 'A'));
  ASSERT_EQ(wcs + 8, wcsrchr(wcs, 'o'));
  ASSERT_EQ(wcs + 12, wcsrchr(wcs, '\0'));
}
