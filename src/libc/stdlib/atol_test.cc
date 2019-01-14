// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#include "gtest/gtest.h"

TEST(atol, examples) {
  ASSERT_EQ(123456, atol("  123456"));
  ASSERT_EQ(-8860, atol("-08860"));
}
