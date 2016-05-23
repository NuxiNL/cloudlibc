// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <testing.h>

TEST(a64l, examples) {
  // Individual characters.
  ASSERT_EQ(0, a64l(""));
  ASSERT_EQ(0, a64l("."));
  ASSERT_EQ(1, a64l("/"));
  ASSERT_EQ(2, a64l("0"));
  ASSERT_EQ(11, a64l("9"));
  ASSERT_EQ(12, a64l("A"));
  ASSERT_EQ(37, a64l("Z"));
  ASSERT_EQ(38, a64l("a"));
  ASSERT_EQ(63, a64l("z"));

  // Random test vectors.
  ASSERT_EQ(-798039413, a64l("98iPE1Trailing garbage"));
  ASSERT_EQ(-457520467, a64l("hegiY1"));
  ASSERT_EQ(-130063910, a64l("O5qDs1"));
  ASSERT_EQ(1146677630, a64l("yZCK2/"));
  ASSERT_EQ(1378758273, a64l("/uW9G/"));
  ASSERT_EQ(1388004411, a64l("vEoiG/"));
  ASSERT_EQ(1464607185, a64l("F50HL/"));
  ASSERT_EQ(1717698504, a64l("6zTMa/"));
  ASSERT_EQ(1783681590, a64l("q6BIe/"));
  ASSERT_EQ(1812451478, a64l("K0x/g/"));
}
