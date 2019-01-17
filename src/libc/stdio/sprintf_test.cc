// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <stdio.h>

#include "gtest/gtest.h"

TEST(sprintf, example) {
  char buf[10];
  ASSERT_EQ(5, sprintf(buf, "Hello"));
  ASSERT_EQ(9, sprintf(buf, "%d + %d = %d", 1, 1, 2));
}
