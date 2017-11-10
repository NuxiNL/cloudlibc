// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>
#include <testing.h>

TEST(atoi, examples) {
  ASSERT_EQ(12, atoi("  12"));
  ASSERT_EQ(-3, atoi("-03"));
  ASSERT_EQ(0, atoi("0x5"));
}
