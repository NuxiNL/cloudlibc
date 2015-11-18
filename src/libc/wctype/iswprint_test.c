// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wctype.h>

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
