// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>
#include <iterator>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(wcslcpy, null) {
  ASSERT_EQ(5, wcslcpy(NULL, L"Hello", 0));
}

TEST(wcslcpy, one) {
  wchar_t buf;
  ASSERT_EQ(6, wcslcpy(&buf, L"Banana", 1));
  ASSERT_EQ(L'\0', buf);
}

TEST(wcslcpy, longer) {
  wchar_t buf[] = L"AAAAAAAAAA";
  ASSERT_EQ(3, wcslcpy(buf, L"Dog", std::size(buf)));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"Dog\0AAAAAA"));
}

TEST(wcslcpy, longest) {
  wchar_t buf[12];
  ASSERT_EQ(23, wcslcpy(buf, L"This is a long sentence", std::size(buf)));
  ASSERT_STREQ(L"This is a l", buf);
}
