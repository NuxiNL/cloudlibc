// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gtest/gtest.h"

TEST(strlen, all) {
  ASSERT_EQ(0, strlen(""));
  ASSERT_EQ(12, strlen("Hello, world"));
}
