// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>
#include <iterator>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(wcslcat, null) {
  ASSERT_EQ(5, wcslcat(NULL, L"Hello", 0));
}

TEST(wcslcat, one) {
  wchar_t buf = L'\0';
  ASSERT_EQ(6, wcslcat(&buf, L"Banana", 1));
  ASSERT_EQ(L'\0', buf);

  buf = L'A';
  ASSERT_EQ(7, wcslcat(&buf, L"Banana", 1));
  ASSERT_EQ(L'A', buf);
}

TEST(wcslcat, longer) {
  wchar_t buf[] = L"AAAAAAAAAAAA";
  ASSERT_EQ(15, wcslcat(buf, L"Foo", std::size(buf) - 1));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"AAAAAAAAAAAA"));

  buf[4] = L'\0';
  ASSERT_EQ(7, wcslcat(buf, L"Bar", std::size(buf) - 1));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"AAAABar\0AAAA"));

  ASSERT_EQ(16, wcslcat(buf, L"Very long", std::size(buf) - 1));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"AAAABarVery\0"));
}
