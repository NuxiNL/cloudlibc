// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wctype.h>

#include "gtest/gtest.h"

TEST(iswblank, examples) {
  ASSERT_FALSE(iswblank(WEOF));

  ASSERT_TRUE(iswblank(L'\t'));
  ASSERT_TRUE(iswblank(L' '));
  ASSERT_TRUE(iswblank(L'\u2001'));

  ASSERT_FALSE(iswblank(L'\n'));
  ASSERT_FALSE(iswblank(L'\u00a0'));
  ASSERT_FALSE(iswblank(L'A'));
  ASSERT_FALSE(iswblank(L'.'));
  ASSERT_FALSE(iswblank(L'€'));
}
