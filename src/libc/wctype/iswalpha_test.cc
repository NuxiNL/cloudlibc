// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wctype.h>

#include "gtest/gtest.h"

TEST(iswalpha, examples) {
  ASSERT_FALSE(iswalpha(WEOF));

  ASSERT_TRUE(iswalpha(L'A'));
  ASSERT_TRUE(iswalpha(L'a'));
  ASSERT_TRUE(iswalpha(L'Å'));
  ASSERT_TRUE(iswalpha(L'Ω'));
  ASSERT_TRUE(iswalpha(L'д'));

  ASSERT_FALSE(iswalpha(L'0'));
  ASSERT_FALSE(iswalpha(L' '));
  ASSERT_FALSE(iswalpha(L'.'));
  ASSERT_FALSE(iswalpha(L'€'));
}
