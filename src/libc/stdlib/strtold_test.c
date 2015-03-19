// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <fenv.h>
#include <float.h>
#include <stdlib.h>
#include <testing.h>

TEST(strtold, hex1) {
  const char *below = "-0x0.ffffffffffffffffffffffffffffffffffffffffffffffffff";
  const char *exact = "-0x1.0";
  const char *above = "-0x1.00000000000000000000000000000000000000000000000001";
#if LDBL_MANT_DIG == 64
  long double low = 0x1.fffffffffffffffep-1L;
  long double high = 0x1.0000000000000002p+0L;
#else
#error "Unknown floating point type"
#endif

  // Test different rounding modes.
  ASSERT_EQ(0, fesetround(FE_DOWNWARD));
  ASSERT_EQ(low, strtold(below + 1, NULL));
  ASSERT_EQ(1.0L, strtold(exact + 1, NULL));
  ASSERT_EQ(1.0L, strtold(above + 1, NULL));
  ASSERT_EQ(-1.0L, strtold(below, NULL));
  ASSERT_EQ(-1.0L, strtold(exact, NULL));
  ASSERT_EQ(-high, strtold(above, NULL));

  ASSERT_EQ(0, fesetround(FE_TONEAREST));
  ASSERT_EQ(1.0L, strtold(below + 1, NULL));
  ASSERT_EQ(1.0L, strtold(exact + 1, NULL));
  ASSERT_EQ(1.0L, strtold(above + 1, NULL));
  ASSERT_EQ(-1.0L, strtold(below, NULL));
  ASSERT_EQ(-1.0L, strtold(exact, NULL));
  ASSERT_EQ(-1.0L, strtold(above, NULL));

  ASSERT_EQ(0, fesetround(FE_TOWARDZERO));
  ASSERT_EQ(low, strtold(below + 1, NULL));
  ASSERT_EQ(1.0L, strtold(exact + 1, NULL));
  ASSERT_EQ(1.0L, strtold(above + 1, NULL));
  ASSERT_EQ(-low, strtold(below, NULL));
  ASSERT_EQ(-1.0L, strtold(exact, NULL));
  ASSERT_EQ(-1.0L, strtold(above, NULL));

  ASSERT_EQ(0, fesetround(FE_UPWARD));
  ASSERT_EQ(1.0L, strtold(below + 1, NULL));
  ASSERT_EQ(1.0L, strtold(exact + 1, NULL));
  ASSERT_EQ(high, strtold(above + 1, NULL));
  ASSERT_EQ(-low, strtold(below, NULL));
  ASSERT_EQ(-1.0L, strtold(exact, NULL));
  ASSERT_EQ(-1.0L, strtold(above, NULL));
}
