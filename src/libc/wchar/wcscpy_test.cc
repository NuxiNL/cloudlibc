// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>
#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS
#include <wchar.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(wcscpy, example) {
  wchar_t buf[] = L"AAAAAAAAAA";
  ASSERT_EQ(buf, wcscpy(buf, L""));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"\0AAAAAAAAA"));
  ASSERT_EQ(buf, wcscpy(buf, L"Hello"));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"Hello\0AAAA"));
  ASSERT_EQ(buf, wcscpy(buf, L""));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"\0ello\0AAAA"));
  ASSERT_EQ(buf, wcscpy(buf, L"Example!!"));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"Example!!\0"));
}
