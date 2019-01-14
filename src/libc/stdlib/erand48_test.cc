// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#include "gtest/gtest.h"

TEST(erand48, example) {
  unsigned short xsubi[3] = {37174, 64810, 11603};
  double d = erand48(xsubi);
  ASSERT_LE(0.896, d);
  ASSERT_GE(0.897, d);
  ASSERT_EQ(22537, xsubi[0]);
  ASSERT_EQ(47966, xsubi[1]);
  ASSERT_EQ(58735, xsubi[2]);
  d = erand48(xsubi);
  ASSERT_LE(0.337, d);
  ASSERT_GE(0.338, d);
  ASSERT_EQ(37344, xsubi[0]);
  ASSERT_EQ(32911, xsubi[1]);
  ASSERT_EQ(22119, xsubi[2]);
  d = erand48(xsubi);
  ASSERT_LE(0.647, d);
  ASSERT_GE(0.648, d);
  ASSERT_EQ(23659, xsubi[0]);
  ASSERT_EQ(29872, xsubi[1]);
  ASSERT_EQ(42445, xsubi[2]);
  d = erand48(xsubi);
  ASSERT_LE(0.500, d);
  ASSERT_GE(0.501, d);
  ASSERT_EQ(31642, xsubi[0]);
  ASSERT_EQ(7875, xsubi[1]);
  ASSERT_EQ(32802, xsubi[2]);
  d = erand48(xsubi);
  ASSERT_LE(0.506, d);
  ASSERT_GE(0.507, d);
  ASSERT_EQ(64669, xsubi[0]);
  ASSERT_EQ(14399, xsubi[1]);
  ASSERT_EQ(33170, xsubi[2]);
}
