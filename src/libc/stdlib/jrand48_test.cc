// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#include "gtest/gtest.h"

TEST(jrand48, example) {
  unsigned short xsubi[3] = {25175, 11052, 45015};
  ASSERT_EQ(1699503220, jrand48(xsubi));
  ASSERT_EQ(2326, xsubi[0]);
  ASSERT_EQ(23668, xsubi[1]);
  ASSERT_EQ(25932, xsubi[2]);
  ASSERT_EQ(-992276007, jrand48(xsubi));
  ASSERT_EQ(41577, xsubi[0]);
  ASSERT_EQ(4569, xsubi[1]);
  ASSERT_EQ(50395, xsubi[2]);
  ASSERT_EQ(-19535776, jrand48(xsubi));
  ASSERT_EQ(31936, xsubi[0]);
  ASSERT_EQ(59488, xsubi[1]);
  ASSERT_EQ(65237, xsubi[2]);
  ASSERT_EQ(79438377, jrand48(xsubi));
  ASSERT_EQ(40395, xsubi[0]);
  ASSERT_EQ(8745, xsubi[1]);
  ASSERT_EQ(1212, xsubi[2]);
  ASSERT_EQ(-1258917728, jrand48(xsubi));
  ASSERT_EQ(37242, xsubi[0]);
  ASSERT_EQ(28832, xsubi[1]);
  ASSERT_EQ(46326, xsubi[2]);
}
