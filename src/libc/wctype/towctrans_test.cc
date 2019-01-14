// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wctype.h>

#include "gtest/gtest.h"

TEST(towctrans, good) {
  wctrans_t wt = wctrans("toupper");
  ASSERT_NE((wctrans_t)0, wt);
  ASSERT_EQ('A', towctrans('a', wt));
}

TEST(towctrans, bad) {
  wctrans_t wt = wctrans("banana");
  ASSERT_EQ((wctrans_t)0, wt);
  ASSERT_EQ('a', towctrans('a', wt));
  ASSERT_EQ('A', towctrans('A', wt));
}
