// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(wcpncpy, null) {
  ASSERT_EQ((wchar_t *)12, wcpncpy((wchar_t *)12, (wchar_t *)500, 0));
}

TEST(wcpncpy, example1) {
  wchar_t buf[] = L"AAAAAAAAAAAA";
  ASSERT_EQ(buf + 5, wcpncpy(buf, L"Hello", 12));
  ASSERT_THAT(buf, testing::ElementsAreArray(L"Hello\0\0\0\0\0\0\0"));
}

TEST(wcpncpy, example2) {
  wchar_t buf[13];
  ASSERT_EQ(buf + 12, wcpncpy(buf, L"This is a very long string", 12));
  buf[12] = '\0';
  ASSERT_THAT(buf, testing::ElementsAreArray(L"This is a ve"));
}
