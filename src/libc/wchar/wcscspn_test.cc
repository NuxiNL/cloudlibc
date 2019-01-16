// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

#include "gtest/gtest.h"

TEST(wcscspn, example) {
  const wchar_t *wcs = L"Hello, world";
  ASSERT_EQ(0, wcscspn(wcs, L"H"));
  ASSERT_EQ(7, wcscspn(wcs, L"rdw"));
  ASSERT_EQ(12, wcscspn(wcs, L"XYZ"));
}
