// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wctype.h>

TEST(iswpunct, examples) {
  ASSERT_FALSE(iswpunct(WEOF));

  ASSERT_TRUE(iswpunct(L'\u00a0'));
  ASSERT_TRUE(iswpunct(L'.'));
  ASSERT_TRUE(iswpunct(L'€'));

  ASSERT_FALSE(iswpunct(L'A'));
  ASSERT_FALSE(iswpunct(L'\t'));
  ASSERT_FALSE(iswpunct(L' '));
  ASSERT_FALSE(iswpunct(L'\u2001'));
  ASSERT_FALSE(iswpunct(L'\n'));
}
