// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <testing.h>

TEST(nrand48, example) {
  unsigned short xsubi[3] = {546, 33817, 23389};
  ASSERT_EQ(914920692, nrand48(xsubi));
  ASSERT_EQ(29829, xsubi[0]);
  ASSERT_EQ(10728, xsubi[1]);
  ASSERT_EQ(27921, xsubi[2]);
  ASSERT_EQ(754104482, nrand48(xsubi));
  ASSERT_EQ(6828, xsubi[0]);
  ASSERT_EQ(28997, xsubi[1]);
  ASSERT_EQ(23013, xsubi[2]);
  ASSERT_EQ(609453945, nrand48(xsubi));
  ASSERT_EQ(58183, xsubi[0]);
  ASSERT_EQ(3826, xsubi[1]);
  ASSERT_EQ(18599, xsubi[2]);
  ASSERT_EQ(1878644360, nrand48(xsubi));
  ASSERT_EQ(36678, xsubi[0]);
  ASSERT_EQ(44304, xsubi[1]);
  ASSERT_EQ(57331, xsubi[2]);
  ASSERT_EQ(2114923686, nrand48(xsubi));
  ASSERT_EQ(58585, xsubi[0]);
  ASSERT_EQ(22861, xsubi[1]);
  ASSERT_EQ(64542, xsubi[2]);
}
