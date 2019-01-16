// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

#include "gtest/gtest.h"

TEST(wcsrchr, examples) {
  const wchar_t *wcs = L"Hello, world";
  ASSERT_EQ(NULL, wcsrchr(wcs, 'A'));
  ASSERT_EQ(wcs + 8, wcsrchr(wcs, 'o'));
  ASSERT_EQ(wcs + 12, wcsrchr(wcs, '\0'));
}
