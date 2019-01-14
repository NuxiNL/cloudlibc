// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#include "gtest/gtest.h"

TEST(drand48, bounds) {
  double value = drand48();
  ASSERT_LE(0.0, value);
  ASSERT_GT(1.0, value);
}
