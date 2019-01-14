// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <limits.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(labs, example) {
  ASSERT_EQ(0, labs(0));
  ASSERT_EQ(42, labs(42));
  ASSERT_EQ(42, labs(-42));
  ASSERT_EQ(LONG_MAX, labs(LONG_MAX));
  ASSERT_EQ(LONG_MAX, labs(-LONG_MAX));
}
