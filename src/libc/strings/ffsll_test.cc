// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <limits.h>
#include <strings.h>

#include "gtest/gtest.h"

TEST(ffsll, examples) {
  ASSERT_EQ(0, ffsll(0x0));
  ASSERT_EQ(1, ffsll(0x3211));
  ASSERT_EQ(2, ffsll(0xabc2));
  ASSERT_EQ(3, ffsll(0x79224));

  ASSERT_EQ(sizeof(long long) * 8 - 3, ffsll(LLONG_MIN >> 3));
  ASSERT_EQ(sizeof(long long) * 8, ffsll(LLONG_MIN));
}
