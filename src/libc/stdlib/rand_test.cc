// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#include "gtest/gtest.h"

TEST(rand, bounds) {
  long value = rand();
  ASSERT_LE(0, value);
  ASSERT_GE(RAND_MAX, value);
}
