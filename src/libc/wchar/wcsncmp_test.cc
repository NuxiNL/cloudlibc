// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

#include "gtest/gtest.h"

TEST(wcsncmp, null) {
  ASSERT_EQ(0, wcsncmp(NULL, NULL, 0));
}

TEST(wcsncmp, examples) {
  ASSERT_EQ(0, wcsncmp(L"", L"", 100));
  ASSERT_EQ(0, wcsncmp(L"Hello", L"Hello", 100));

  ASSERT_EQ(0, wcsncmp(L"Hello", L"Hello, world", 5));
  ASSERT_GT(0, wcsncmp(L"Hello", L"Hello, world", 6));
  ASSERT_LT(0, wcsncmp(L"Hello, world", L"Hello", 100));

  ASSERT_EQ(0, wcsncmp(L"Hello!", L"Hello.", 5));
  ASSERT_GT(0, wcsncmp(L"Hello!", L"Hello.", 6));
  ASSERT_LT(0, wcsncmp(L"Hello.", L"Hello!", 100));
}
