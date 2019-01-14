// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fenv.h>
#include <float.h>
#include <locale.h>
#include <math.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(strtof, dec1) {
  // Number without an exponent, but with a radix character.
  const char *str = "0.0625";
  char *endptr;
  ASSERT_EQ(0.0625f, strtof(str, NULL));
  ASSERT_EQ(0.0625f, strtof(str, &endptr));
  ASSERT_EQ(str + 6, endptr);
}

TEST(strtof, dec2) {
  // Number with an exponent, but no radix character.
  const char *str = "12800e-2";
  char *endptr;
  ASSERT_EQ(128.0f, strtof(str, NULL));
  ASSERT_EQ(128.0f, strtof(str, &endptr));
  ASSERT_EQ(str + 8, endptr);
}

TEST(strtof, dec3) {
  // Comma as a radix character. Parsing should stop at the comma.
  const char *str = "7,5";
  char *endptr;
  ASSERT_EQ(7.0f, strtof(str, &endptr));
  ASSERT_EQ(str + 1, endptr);

  // Except when we use the proper locale.
  locale_t locale = newlocale(LC_NUMERIC_MASK, "nl_NL", 0);
  ASSERT_NE((locale_t)0, locale);

  ASSERT_EQ(7.5f, strtof_l(str, &endptr, locale));
  ASSERT_EQ(str + 3, endptr);

  str = ",75";
  ASSERT_EQ(.75f, strtof_l(str, &endptr, locale));
  ASSERT_EQ(str + 3, endptr);

  freelocale(locale);
}

TEST(strtof, hex1) {
  // We should preserve all 24 bits of the significand.
  const char *str = "  0xcaf.eff";
  char *endptr;
  ASSERT_EQ(0xcaf.effp0, strtof(str, NULL));
  ASSERT_EQ(0xcaf.effp0, strtof(str, &endptr));
  ASSERT_EQ(str + 11, endptr);
}

TEST(strtof, hex2) {
  // Should evaluate to zero.
  const char *str = "0x0p99999999999999999999";
  char *endptr;
  ASSERT_EQ(0.0, strtof(str, &endptr));
  ASSERT_EQ(str + 24, endptr);
}

TEST(strtof, hex3) {
  // Would overflow, so it should return HUGE_VALF.
  const char *str = "\t0x1p+30000";
  char *endptr;
  ASSERT_EQ(HUGE_VALF, strtof(str, &endptr));
  ASSERT_EQ(str + 11, endptr);
}

TEST(strtof, hex4) {
  // Would underflow, so it should return 0.0f.
  const char *str = "\n0X1P-30000 ";
  char *endptr;
  ASSERT_EQ(0.0f, strtof(str, &endptr));
  ASSERT_EQ(str + 11, endptr);
}

TEST(strtof, hex5) {
  // Negative numbers.
  const char *str = "-0x123xyz";
  char *endptr;
  ASSERT_EQ(-0x123.0p0, strtof(str, &endptr));
  ASSERT_EQ(str + 6, endptr);
}

TEST(strtof, hex6) {
  // The 0x should not be matched. Instead, only the zero is procesed.
  const char *str = "0x";
  char *endptr;
  ASSERT_EQ(0, strtof(str, &endptr));
  ASSERT_EQ(str + 1, endptr);
}

TEST(strtof, hex7) {
  // First digit can be placed after the radix character.
  const char *str = "0x.8";
  char *endptr;
  ASSERT_EQ(0.5, strtof(str, &endptr));
  ASSERT_EQ(str + 4, endptr);
}

TEST(strtof, hex8) {
  const char *below = "-0x0.ffffffffffffffffffffffffffffffffffffffffffffffffff";
  const char *exact = "-0x1.0";
  const char *above = "-0x1.00000000000000000000000000000000000000000000000001";
#if FLT_MANT_DIG == 24
  float low = 0x1.fffffep-1f;
  float high = 0x1.000002p+0f;
#else
#error "Unknown floating point type"
#endif

  // Test different rounding modes.
  ASSERT_EQ(0, fesetround(FE_DOWNWARD));
  ASSERT_EQ(low, strtof(below + 1, NULL));
  ASSERT_EQ(1.0f, strtof(exact + 1, NULL));
  ASSERT_EQ(1.0f, strtof(above + 1, NULL));
  ASSERT_EQ(-1.0f, strtof(below, NULL));
  ASSERT_EQ(-1.0f, strtof(exact, NULL));
  ASSERT_EQ(-high, strtof(above, NULL));

  ASSERT_EQ(0, fesetround(FE_TONEAREST));
  ASSERT_EQ(1.0f, strtof(below + 1, NULL));
  ASSERT_EQ(1.0f, strtof(exact + 1, NULL));
  ASSERT_EQ(1.0f, strtof(above + 1, NULL));
  ASSERT_EQ(-1.0f, strtof(below, NULL));
  ASSERT_EQ(-1.0f, strtof(exact, NULL));
  ASSERT_EQ(-1.0f, strtof(above, NULL));

  ASSERT_EQ(0, fesetround(FE_TOWARDZERO));
  ASSERT_EQ(low, strtof(below + 1, NULL));
  ASSERT_EQ(1.0f, strtof(exact + 1, NULL));
  ASSERT_EQ(1.0f, strtof(above + 1, NULL));
  ASSERT_EQ(-low, strtof(below, NULL));
  ASSERT_EQ(-1.0f, strtof(exact, NULL));
  ASSERT_EQ(-1.0f, strtof(above, NULL));

  ASSERT_EQ(0, fesetround(FE_UPWARD));
  ASSERT_EQ(1.0f, strtof(below + 1, NULL));
  ASSERT_EQ(1.0f, strtof(exact + 1, NULL));
  ASSERT_EQ(high, strtof(above + 1, NULL));
  ASSERT_EQ(-low, strtof(below, NULL));
  ASSERT_EQ(-1.0f, strtof(exact, NULL));
  ASSERT_EQ(-1.0f, strtof(above, NULL));

  ASSERT_EQ(0, fesetround(FE_TONEAREST));
}

#if FLT_HAS_SUBNORM == 1
TEST(strtof, hex9) {
#if FLT_MANT_DIG == 24
  const char *normal = "0x1p-126";
  const char *highest_subnormal = "0x1.fffffcp-127";
  float high = 0x1.fffffcp-127;
  const char *lowest_subnormal = "0x1p-149";
  const char *underflow = "0x1p-150";
#else
#error "Unknown floating point type"
#endif

  // Test parsing of subnormal values.
  errno = 0;
  ASSERT_EQ(FLT_MIN, strtof(normal, NULL));
  ASSERT_EQ(0, errno);
  ASSERT_EQ(high, strtof(highest_subnormal, NULL));
  ASSERT_EQ(0, errno);
  ASSERT_EQ(FLT_TRUE_MIN, strtof(lowest_subnormal, NULL));
  ASSERT_EQ(0, errno);
  ASSERT_EQ(0.0, strtof(underflow, NULL));
  ASSERT_EQ(ERANGE, errno);
}
#endif

TEST(strtof, nan1) {
  // NAN.
  const char *str = "NaN(Hello";
  char *endptr;
  ASSERT_TRUE(isnan(strtof(str, &endptr)));
  ASSERT_EQ(str + 3, endptr);
}

TEST(strtof, nan2) {
  // NAN(...).
  const char *str = "NaN(Hello world) :-)";
  char *endptr;
  ASSERT_TRUE(isnan(strtof(str, &endptr)));
  ASSERT_EQ(str + 16, endptr);
}

TEST(strtof, inf1) {
  // INF.
  const char *str = "INFINITE";
  char *endptr;
  ASSERT_EQ(INFINITY, strtof(str, &endptr));
  ASSERT_EQ(str + 3, endptr);
}

TEST(strtof, inf2) {
  // INFINITY.
  const char *str = "-INFINITY";
  char *endptr;
  ASSERT_EQ(-INFINITY, strtof(str, &endptr));
  ASSERT_EQ(str + 9, endptr);
}

TEST(strtof, huge_val1) {
  // Too large for a 32-bits floating point number.
  const char *str =
      "10000000000000000000000000000000000000000000000000000000000000000000000"
      "00000000000000000000000000000000000000000000000000000000000000000000000"
      "00000000000000000000000000000000000000000000000000000000000000000000000";
  char *endptr;
  ASSERT_EQ(HUGE_VALF, strtof(str, &endptr));
  ASSERT_EQ(str + 213, endptr);
  ASSERT_EQ(ERANGE, errno);
}

TEST(strtof, huge_val2) {
  // Too large for a 32-bits floating point number.
  const char *str = "-1e3000";
  char *endptr;
  ASSERT_EQ(-HUGE_VALF, strtof(str, &endptr));
  ASSERT_EQ(str + 7, endptr);
  ASSERT_EQ(ERANGE, errno);
}

TEST(strtof, zero1) {
  // Too small for a 32-bits floating point number.
  const char *str =
      "0.000000000000000000000000000000000000000000000000000000000000000000000"
      "00000000000000000000000000000000000000000000000000000000000000000000000"
      "00000000000000000000000000000000000000000000000000000000000000000000001";
  char *endptr;
  float v = strtof(str, &endptr);
  ASSERT_EQ(0.0, v);
  ASSERT_FALSE(signbit(v));
  ASSERT_EQ(str + 213, endptr);
  ASSERT_EQ(ERANGE, errno);
}

TEST(strtof, zero2) {
  // Too small for a 32-bits floating point number.
  const char *str = "-1e-3000";
  char *endptr;
  float v = strtof(str, &endptr);
  ASSERT_EQ(0.0, v);
  ASSERT_TRUE(signbit(v));
  ASSERT_EQ(str + 8, endptr);
  ASSERT_EQ(ERANGE, errno);
}

TEST(strtof, zero3) {
  // Actual zero.
  errno = 0;
  const char *str = "0.0";
  char *endptr;
  float v = strtof(str, &endptr);
  ASSERT_EQ(0.0, v);
  ASSERT_FALSE(signbit(v));
  ASSERT_EQ(str + 3, endptr);
  ASSERT_EQ(0, errno);
}

TEST(strtof, zero4) {
  // Actual zero.
  errno = 0;
  const char *str = "-0.0";
  char *endptr;
  float v = strtof(str, &endptr);
  ASSERT_EQ(0.0, v);
  ASSERT_TRUE(signbit(v));
  ASSERT_EQ(str + 4, endptr);
  ASSERT_EQ(0, errno);
}
