// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wctype.h>

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
