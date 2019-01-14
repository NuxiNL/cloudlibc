// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <inttypes.h>

#include "gtest/gtest.h"

TEST(imaxabs, examples) {
  ASSERT_EQ(5, imaxabs(5));
  ASSERT_EQ(5, imaxabs(-5));

  ASSERT_EQ(INTMAX_MAX, imaxabs(INTMAX_MAX));
  ASSERT_EQ(INTMAX_MAX, imaxabs(-INTMAX_MAX));
}
