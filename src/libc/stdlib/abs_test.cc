// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <limits.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(abs, example) {
  ASSERT_EQ(0, abs(0));
  ASSERT_EQ(42, abs(42));
  ASSERT_EQ(42, abs(-42));
  ASSERT_EQ(INT_MAX, abs(INT_MAX));
  ASSERT_EQ(INT_MAX, abs(-INT_MAX));
}
