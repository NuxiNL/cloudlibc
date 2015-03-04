// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wctype.h>

TEST(towctrans, good) {
  wctrans_t wt = wctrans("toupper");
  ASSERT_NE(0, wt);
  ASSERT_EQ('A', towctrans('a', wt));
}

TEST(towctrans, bad) {
  wctrans_t wt = wctrans("banana");
  ASSERT_EQ(0, wt);
  ASSERT_EQ('a', towctrans('a', wt));
  ASSERT_EQ('A', towctrans('A', wt));
}
