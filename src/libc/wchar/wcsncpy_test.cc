// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(wcsncpy, null) {
  ASSERT_EQ((wchar_t *)12, wcsncpy((wchar_t *)12, (wchar_t *)500, 0));
}

TEST(wcsncpy, example1) {
  wchar_t buf[] = L"AAAAAAAAAAAA";
  ASSERT_EQ(buf, wcsncpy(buf, L"Hello", 12));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"Hello\0\0\0\0\0\0\0"));
}

TEST(wcsncpy, example2) {
  wchar_t buf[13];
  ASSERT_EQ(buf, wcsncpy(buf, L"This is a very long string", 12));
  buf[12] = L'\0';
  ASSERT_THAT(buf, testing::ElementsAreArray(L"This is a ve"));
}
