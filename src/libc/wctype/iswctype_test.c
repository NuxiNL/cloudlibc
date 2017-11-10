// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <testing.h>
#include <wctype.h>

TEST(iswctype, good) {
  wctype_t wt = wctype("upper");
  ASSERT_NE(0, wt);
  ASSERT_TRUE(iswctype('A', wt));
  ASSERT_FALSE(iswctype('a', wt));
}

TEST(iswctype, bad) {
  wctype_t wt = wctype("banana");
  ASSERT_EQ(0, wt);
  ASSERT_FALSE(iswctype('p', wt));
}
