// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#include "gtest/gtest.h"

TEST(random, bounds) {
  long value = random();
  ASSERT_LE(0, value);
  ASSERT_GE(2147483647, value);
}
