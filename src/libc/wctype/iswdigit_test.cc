// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wctype.h>

#include "gtest/gtest.h"

TEST(iswdigit, examples) {
  ASSERT_FALSE(iswdigit(WEOF));

  ASSERT_TRUE(iswdigit(L'0'));
  ASSERT_TRUE(iswdigit(L'1'));
  ASSERT_TRUE(iswdigit(L'2'));
  ASSERT_TRUE(iswdigit(L'3'));
  ASSERT_TRUE(iswdigit(L'4'));
  ASSERT_TRUE(iswdigit(L'5'));
  ASSERT_TRUE(iswdigit(L'6'));
  ASSERT_TRUE(iswdigit(L'7'));
  ASSERT_TRUE(iswdigit(L'8'));
  ASSERT_TRUE(iswdigit(L'9'));

  ASSERT_FALSE(iswdigit(L'A'));
  ASSERT_FALSE(iswdigit(L'①'));
  ASSERT_FALSE(iswdigit(L'?'));
}
