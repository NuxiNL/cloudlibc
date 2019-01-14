// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wctype.h>

#include "gtest/gtest.h"

TEST(iswprint, examples) {
  ASSERT_FALSE(iswprint(WEOF));

  ASSERT_TRUE(iswprint(L' '));
  ASSERT_TRUE(iswprint(L'\u2001'));
  ASSERT_TRUE(iswprint(L'\u00a0'));
  ASSERT_TRUE(iswprint(L'A'));
  ASSERT_TRUE(iswprint(L'.'));
  ASSERT_TRUE(iswprint(L'€'));

  ASSERT_FALSE(iswprint(L'\t'));
  ASSERT_FALSE(iswprint(L'\n'));
}
