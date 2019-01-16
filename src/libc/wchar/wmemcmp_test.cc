// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>
#include <iterator>

#include "gtest/gtest.h"

TEST(wmemcmp, null) {
  ASSERT_EQ(0, wmemcmp(NULL, NULL, 0));
}

TEST(wmemcmp, example) {
  const wchar_t buf1[] = L"Hello";
  const wchar_t buf2[] = L"Helxo";
  ASSERT_EQ(0, wmemcmp(buf1, buf1, std::size(buf1)));
  ASSERT_GT(0, wmemcmp(buf1, buf2, std::size(buf1)));
  ASSERT_LT(0, wmemcmp(buf2, buf1, std::size(buf1)));
}
