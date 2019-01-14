// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#include "gtest/gtest.h"

TEST(ldiv, examples) {
  ldiv_t r = ldiv(5, 2);
  ASSERT_EQ(2, r.quot);
  ASSERT_EQ(1, r.rem);

  r = ldiv(-11, 2);
  ASSERT_EQ(-5, r.quot);
  ASSERT_EQ(-1, r.rem);

  r = ldiv(-43, -3);
  ASSERT_EQ(14, r.quot);
  ASSERT_EQ(-1, r.rem);

  r = ldiv(18, -7);
  ASSERT_EQ(-2, r.quot);
  ASSERT_EQ(4, r.rem);
}
