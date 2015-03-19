// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
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

#if LDBL_HAS_SUBNORM == 1
TEST(strtold, hex2) {
#if LDBL_MANT_DIG == 64
  const char *normal = "0x1p-16382";
  const char *highest_subnormal = "0x1.fffffffffffffffcp-16383";
  long double high = 0x1.fffffffffffffffcp-16383L;
  const char *lowest_subnormal = "0x1p-16445";
  const char *underflow = "0x1p-16446";
#else
#error "Unknown floating point type"
#endif

  // Test parsing of subnormal values.
  ASSERT_EQ(LDBL_MIN, strtold(normal, NULL));
  ASSERT_EQ(0, errno);
  ASSERT_EQ(high, strtold(highest_subnormal, NULL));
  ASSERT_EQ(0, errno);
  ASSERT_EQ(LDBL_TRUE_MIN, strtold(lowest_subnormal, NULL));
  ASSERT_EQ(0, errno);
  ASSERT_EQ(0.0, strtold(underflow, NULL));
  ASSERT_EQ(ERANGE, errno);
}
#endif
