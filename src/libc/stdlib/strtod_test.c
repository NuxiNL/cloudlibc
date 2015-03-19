// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <fenv.h>
#include <float.h>
#include <stdlib.h>
#include <testing.h>

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
}
