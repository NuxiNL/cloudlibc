// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gtest/gtest.h"

TEST(strnlen, null) {
  ASSERT_EQ(0, strnlen(NULL, 0));
  ASSERT_EQ(0, strnlen("", 100));
  ASSERT_EQ(7, strnlen("Hello, world", 7));
}
