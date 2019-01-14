// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fenv.h>
#include <float.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(strtod, hex1) {
  const char *below = "-0x0.ffffffffffffffffffffffffffffffffffffffffffffffffff";
  const char *exact = "-0x1.0";
  const char *above = "-0x1.00000000000000000000000000000000000000000000000001";
#if DBL_MANT_DIG == 53
  double low = 0x1.fffffffffffffp-1;
  double high = 0x1.0000000000001p+0;
#else
#error "Unknown floating point type"
#endif

  // Test different rounding modes.
  ASSERT_EQ(0, fesetround(FE_DOWNWARD));
  ASSERT_EQ(low, strtod(below + 1, NULL));
  ASSERT_EQ(1.0, strtod(exact + 1, NULL));
  ASSERT_EQ(1.0, strtod(above + 1, NULL));
  ASSERT_EQ(-1.0, strtod(below, NULL));
  ASSERT_EQ(-1.0, strtod(exact, NULL));
  ASSERT_EQ(-high, strtod(above, NULL));

  ASSERT_EQ(0, fesetround(FE_TONEAREST));
  ASSERT_EQ(1.0, strtod(below + 1, NULL));
  ASSERT_EQ(1.0, strtod(exact + 1, NULL));
  ASSERT_EQ(1.0, strtod(above + 1, NULL));
  ASSERT_EQ(-1.0, strtod(below, NULL));
  ASSERT_EQ(-1.0, strtod(exact, NULL));
  ASSERT_EQ(-1.0, strtod(above, NULL));

  ASSERT_EQ(0, fesetround(FE_TOWARDZERO));
  ASSERT_EQ(low, strtod(below + 1, NULL));
  ASSERT_EQ(1.0, strtod(exact + 1, NULL));
  ASSERT_EQ(1.0, strtod(above + 1, NULL));
  ASSERT_EQ(-low, strtod(below, NULL));
  ASSERT_EQ(-1.0, strtod(exact, NULL));
  ASSERT_EQ(-1.0, strtod(above, NULL));

  ASSERT_EQ(0, fesetround(FE_UPWARD));
  ASSERT_EQ(1.0, strtod(below + 1, NULL));
  ASSERT_EQ(1.0, strtod(exact + 1, NULL));
  ASSERT_EQ(high, strtod(above + 1, NULL));
  ASSERT_EQ(-low, strtod(below, NULL));
  ASSERT_EQ(-1.0, strtod(exact, NULL));
  ASSERT_EQ(-1.0, strtod(above, NULL));

  ASSERT_EQ(0, fesetround(FE_TONEAREST));
}

#if DBL_HAS_SUBNORM == 1
TEST(strtod, hex2) {
#if DBL_MANT_DIG == 53
  const char *normal = "0x1p-1022";
  const char *highest_subnormal = "0X1.fFfFfFfFfFfFEP-1023";
  double high = 0x1.ffffffffffffep-1023;
  const char *lowest_subnormal = "0x1p-1074";
  const char *underflow = "0x1p-1075";
  const char *above_subnormal = "0x1.ffffffffffffe000001p-1023";
#else
#error "Unknown floating point type"
#endif

  // Test parsing of subnormal values.
  errno = 0;
  ASSERT_EQ(DBL_MIN, strtod(normal, NULL));
  ASSERT_EQ(0, errno);
  ASSERT_EQ(high, strtod(highest_subnormal, NULL));
  ASSERT_EQ(0, errno);
  ASSERT_EQ(DBL_TRUE_MIN, strtod(lowest_subnormal, NULL));
  ASSERT_EQ(0, errno);
  ASSERT_EQ(0.0, strtod(underflow, NULL));
  ASSERT_EQ(ERANGE, errno);

  // Test value between normal and subnormal.
  ASSERT_EQ(0, fesetround(FE_DOWNWARD));
  ASSERT_EQ(high, strtod(above_subnormal, NULL));
  ASSERT_EQ(0, fesetround(FE_TONEAREST));
  ASSERT_EQ(high, strtod(above_subnormal, NULL));
  ASSERT_EQ(0, fesetround(FE_TOWARDZERO));
  ASSERT_EQ(high, strtod(above_subnormal, NULL));
  ASSERT_EQ(0, fesetround(FE_UPWARD));
  ASSERT_EQ(DBL_MIN, strtod(above_subnormal, NULL));

  ASSERT_EQ(0, fesetround(FE_TONEAREST));
}
#endif
