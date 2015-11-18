// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wctype.h>

TEST(iswcntrl, examples) {
  ASSERT_FALSE(iswcntrl(WEOF));

  ASSERT_TRUE(iswcntrl(L'\0'));
  ASSERT_TRUE(iswcntrl(L'\n'));
  ASSERT_TRUE(iswcntrl(L'\t'));

  ASSERT_FALSE(iswcntrl(L'0'));
  ASSERT_FALSE(iswcntrl(L'A'));
  ASSERT_FALSE(iswcntrl(L' '));
  ASSERT_FALSE(iswcntrl(L'.'));
  ASSERT_FALSE(iswcntrl(L'€'));
}
