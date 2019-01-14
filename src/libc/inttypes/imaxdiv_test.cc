// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <inttypes.h>

#include "gtest/gtest.h"

TEST(imaxdiv, examples) {
  imaxdiv_t r = imaxdiv(-44, 7);
  ASSERT_EQ(-6, r.quot);
  ASSERT_EQ(-2, r.rem);

  r = imaxdiv(INTMAX_MIN, INTMAX_MIN);
  ASSERT_EQ(1, r.quot);
  ASSERT_EQ(0, r.rem);
}
