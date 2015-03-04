// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <fenv.h>
#include <float.h>
#include <testing.h>

TEST(fesetround, flt_rounds) {
  // FLT_ROUNDS should be in sync with fesetround().
  ASSERT_EQ(0, fesetround(FE_TOWARDZERO));
  ASSERT_EQ(FE_TOWARDZERO, fegetround());
  ASSERT_EQ(0, FLT_ROUNDS);

  ASSERT_EQ(0, fesetround(FE_TONEAREST));
  ASSERT_EQ(FE_TONEAREST, fegetround());
  ASSERT_EQ(1, FLT_ROUNDS);

  ASSERT_EQ(0, fesetround(FE_UPWARD));
  ASSERT_EQ(FE_UPWARD, fegetround());
  ASSERT_EQ(2, FLT_ROUNDS);

  ASSERT_EQ(0, fesetround(FE_DOWNWARD));
  ASSERT_EQ(FE_DOWNWARD, fegetround());
  ASSERT_EQ(3, FLT_ROUNDS);
}
