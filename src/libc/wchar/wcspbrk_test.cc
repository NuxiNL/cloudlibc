// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

#include "gtest/gtest.h"

TEST(wcspbrk, example) {
  const wchar_t *wcs = L"Hello, world";
  ASSERT_EQ(wcs, wcspbrk(wcs, L"H"));
  ASSERT_EQ(wcs + 7, wcspbrk(wcs, L"rdw"));
  ASSERT_EQ(NULL, wcspbrk(wcs, L"XYZ"));
}
