// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wctype.h>

TEST(iswspace, examples) {
  ASSERT_FALSE(iswspace(WEOF));

  ASSERT_TRUE(iswspace(L'\t'));
  ASSERT_TRUE(iswspace(L' '));
  ASSERT_TRUE(iswspace(L'\u2001'));
  ASSERT_TRUE(iswspace(L'\n'));

  ASSERT_FALSE(iswspace(L'\u00a0'));
  ASSERT_FALSE(iswspace(L'A'));
  ASSERT_FALSE(iswspace(L'.'));
  ASSERT_FALSE(iswspace(L'€'));
}
