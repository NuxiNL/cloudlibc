// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wctype.h>

TEST(iswalpha, examples) {
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
