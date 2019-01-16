// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <wchar.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(wcsncat, example) {
  wchar_t buf[11] = L"\0AAAAAAAAA";
  ASSERT_EQ(buf, wcsncat(buf, L"", 0));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"\0AAAAAAAAA"));
  ASSERT_EQ(buf, wcsncat(buf, L"Hello", 99999));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"Hello\0AAAA"));
  ASSERT_EQ(buf, wcsncat(buf, L"", 1));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"Hello\0AAAA"));
  ASSERT_EQ(buf, wcsncat(buf, L"!!!!!!!!!!!!", 3));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"Hello!!!\0A"));
}
