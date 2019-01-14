// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#include "gtest/gtest.h"

TEST(mrand48, bounds) {
  long value = mrand48();
  ASSERT_LE(-2147483648, value);
  ASSERT_GE(2147483647, value);
}
