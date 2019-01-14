// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <limits.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(llabs, example) {
  ASSERT_EQ(0, llabs(0));
  ASSERT_EQ(42, llabs(42));
  ASSERT_EQ(42, llabs(-42));
  ASSERT_EQ(LLONG_MAX, llabs(LLONG_MAX));
  ASSERT_EQ(LLONG_MAX, llabs(-LLONG_MAX));
}
