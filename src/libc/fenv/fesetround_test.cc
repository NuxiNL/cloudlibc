// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <fenv.h>
#include <float.h>

#include "gtest/gtest.h"

// TODO(ed): Re-enable this test on aarch64 and arm once Clang's
// __builtin_flt_rounds() is fixed.
// https://llvm.org/bugs/show_bug.cgi?id=25191
// https://llvm.org/bugs/show_bug.cgi?id=30439
#if !defined(__aarch64__) && !defined(__arm__)
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
#endif
