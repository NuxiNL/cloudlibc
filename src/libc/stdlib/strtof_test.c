// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <testing.h>

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
  // Would overflow, so it should return HUGE_VAL.
  const char *str = "\t0x1p+30000";
  char *endptr;
  ASSERT_EQ(HUGE_VAL, strtof(str, &endptr));
  ASSERT_EQ(str + 11, endptr);
}

TEST(strtof, hex4) {
  // Would underflow, so it should return FLT_MIN.
  // TODO(edje): Let this return FLT_TRUE_MIN.
  const char *str = "\n0X1P-30000 ";
  char *endptr;
  ASSERT_EQ(FLT_MIN, strtof(str, &endptr));
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
  // Should not match.
  const char *str = "0x";
  char *endptr;
  ASSERT_EQ(0, strtof(str, &endptr));
  ASSERT_EQ(EINVAL, errno);
  ASSERT_EQ(str, endptr);
}

TEST(strtof, hex7) {
  // First digit can be placed after the radix character.
  const char *str = "0x.8";
  char *endptr;
  ASSERT_EQ(0.5, strtof(str, &endptr));
  ASSERT_EQ(str + 4, endptr);
}

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
