// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

#include "gtest/gtest.h"

TEST(wcsspn, example) {
  const wchar_t *wcs = L"Hello, world";
  ASSERT_EQ(0, wcsspn(wcs, L""));
  ASSERT_EQ(0, wcsspn(wcs, L"Foo"));
  ASSERT_EQ(5, wcsspn(wcs, L"olHe"));
  ASSERT_EQ(12, wcsspn(wcs, L"Helo, wrld"));
}
