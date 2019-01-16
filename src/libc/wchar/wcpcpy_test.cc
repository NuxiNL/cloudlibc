// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <wchar.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(wcpcpy, example) {
  wchar_t buf[] = L"AAAAAAAAAA";
  ASSERT_EQ(buf, wcpcpy(buf, L""));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"\0AAAAAAAAA"));
  ASSERT_EQ(buf + 5, wcpcpy(buf, L"Hello"));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"Hello\0AAAA"));
  ASSERT_EQ(buf, wcpcpy(buf, L""));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"\0ello\0AAAA"));
  ASSERT_EQ(buf + 9, wcpcpy(buf, L"Example!!"));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"Example!!\0"));
}
