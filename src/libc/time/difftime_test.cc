// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <time.h>

#include "gtest/gtest.h"

TEST(difftime, examples) {
  // Mixed signedness.
  ASSERT_EQ(2000, difftime(1000, -1000));
  ASSERT_EQ(-2000, difftime(-1000, 1000));

  ASSERT_EQ(-2, difftime(1414143811, 1414143813));
  ASSERT_EQ(-1, difftime(1414143812, 1414143813));
  ASSERT_EQ(0, difftime(1414143813, 1414143813));
  ASSERT_EQ(1, difftime(1414143814, 1414143813));
  ASSERT_EQ(2, difftime(1414143815, 1414143813));
}
