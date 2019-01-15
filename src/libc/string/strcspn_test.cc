// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gtest/gtest.h"

TEST(strcspn, example) {
  const char *str = "Hello, world";
  ASSERT_EQ(0, strcspn(str, "H"));
  ASSERT_EQ(7, strcspn(str, "rdw"));
  ASSERT_EQ(12, strcspn(str, "XYZ"));
}
