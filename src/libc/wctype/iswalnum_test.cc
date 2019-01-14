// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wctype.h>

#include "gtest/gtest.h"

TEST(iswalnum, examples) {
  ASSERT_FALSE(iswalnum(WEOF));

  ASSERT_TRUE(iswalnum(L'0'));
  ASSERT_TRUE(iswalnum(L'A'));
  ASSERT_TRUE(iswalnum(L'a'));
  ASSERT_TRUE(iswalnum(L'Å'));
  ASSERT_TRUE(iswalnum(L'Ω'));
  ASSERT_TRUE(iswalnum(L'д'));

  ASSERT_FALSE(iswalnum(L' '));
  ASSERT_FALSE(iswalnum(L'.'));
  ASSERT_FALSE(iswalnum(L'€'));
}
