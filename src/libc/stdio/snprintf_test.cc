// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <fenv.h>
#include <float.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>

#include "gtest/gtest.h"

#define TEST_OUTPUT(out, ...)                                            \
  do {                                                                   \
    char buf[sizeof(out)];                                               \
    ASSERT_EQ(sizeof(buf) - 1, snprintf(buf, sizeof(buf), __VA_ARGS__)); \
    ASSERT_STREQ(out, buf);                                              \
  } while (0);

TEST(snprintf, null) {
  ASSERT_EQ(12, snprintf(NULL, 0, "Hello, world"));
}

TEST(snprintf, null_string) {
  char *ptr = NULL;
  TEST_OUTPUT("Hello(nuworld", "Hello%.3sworld", ptr);
}

TEST(snprintf, truncation) {
  char buf[10];
  ASSERT_EQ(12, snprintf(buf, sizeof(buf), "Hello, world"));
  ASSERT_STREQ("Hello, wo", buf);
}

TEST(snprintf, number_width_precision_left) {
  TEST_OUTPUT("          0000000005", "%20.10d", 5);
}

TEST(snprintf, number_width_precision_right) {
  TEST_OUTPUT("0000000005          ", "%-20.10d", 5);
}

TEST(snprintf, grouping_en_us) {
  locale_t locale = newlocale(LC_NUMERIC_MASK, "en_US", 0);
  ASSERT_NE((locale_t)0, locale);
  char buf[21];
  ASSERT_EQ(20, snprintf_l(buf, sizeof(buf), locale, "%'020d", 12345678));
  ASSERT_STREQ("000000000012,345,678", buf);
  freelocale(locale);
}

TEST(snprintf, grouping_ru_ru) {
  locale_t locale = newlocale(LC_NUMERIC_MASK, "ru_RU", 0);
  ASSERT_NE((locale_t)0, locale);
  char buf[21];
  ASSERT_EQ(20, snprintf_l(buf, sizeof(buf), locale, "%'020d", -123456789));
  ASSERT_STREQ("-00000000123 456 789", buf);
  freelocale(locale);
}

TEST(snprintf, numbered_arguments) {
  TEST_OUTPUT("7 5", "%2$d %1$d", 5, 7);
}

TEST(snprintf, hex_left) {
  TEST_OUTPUT("0x0000001337        ", "%-#20.10x", 0x1337);
}

TEST(snprintf, hex_right) {
  TEST_OUTPUT("        0x0000001337", "%#20.10x", 0x1337);
}

TEST(snprintf, octal1) {
  TEST_OUTPUT("1", "%o", 1);
}

TEST(snprintf, octal2) {
  TEST_OUTPUT("01", "%#o", 1);
}

TEST(snprintf, float10_f_g) {
  // No modifiers.
  TEST_OUTPUT("|1.000000|", "|%f|", 1.0);
  TEST_OUTPUT("|1.000000|", "|%#f|", 1.0);
  TEST_OUTPUT("|1|", "|%g|", 1.0);
  TEST_OUTPUT("|1.00000|", "|%#g|", 1.0);

  // Field width.
  TEST_OUTPUT("|  1.000000|", "|%10f|", 1.0);
  TEST_OUTPUT("|  1.000000|", "|%#10f|", 1.0);
  TEST_OUTPUT("|         1|", "|%10g|", 1.0);
  TEST_OUTPUT("|   1.00000|", "|%#10g|", 1.0);

  // Left justified.
  TEST_OUTPUT("|1.000000  |", "|%-10f|", 1.0);
  TEST_OUTPUT("|1.000000  |", "|%#-10f|", 1.0);
  TEST_OUTPUT("|1         |", "|%-10g|", 1.0);
  TEST_OUTPUT("|1.00000   |", "|%#-10g|", 1.0);

  // Zero padded.
  TEST_OUTPUT("|001.000000|", "|%010f|", 1.0);
  TEST_OUTPUT("|001.000000|", "|%#010f|", 1.0);
  TEST_OUTPUT("|0000000001|", "|%010g|", 1.0);
  TEST_OUTPUT("|0001.00000|", "|%#010g|", 1.0);

  // No decimals.
  TEST_OUTPUT("|         1|", "|%10.0f|", 1.0);
  TEST_OUTPUT("|        1.|", "|%#10.0f|", 1.0);
  TEST_OUTPUT("|         1|", "|%10.0g|", 1.0);
  TEST_OUTPUT("|        1.|", "|%#10.0g|", 1.0);

  // No decimals, left justified.
  TEST_OUTPUT("|1         |", "|%-10.0f|", 1.0);
  TEST_OUTPUT("|1.        |", "|%#-10.0f|", 1.0);
  TEST_OUTPUT("|1         |", "|%-10.0g|", 1.0);
  TEST_OUTPUT("|1.        |", "|%#-10.0g|", 1.0);

  // No decimals, zero padded.
  TEST_OUTPUT("|0000000001|", "|%010.0f|", 1.0);
  TEST_OUTPUT("|000000001.|", "|%#010.0f|", 1.0);
  TEST_OUTPUT("|0000000001|", "|%010.0g|", 1.0);
  TEST_OUTPUT("|000000001.|", "|%#010.0g|", 1.0);
}

TEST(snprintf, float10_e_zero) {
  TEST_OUTPUT("0.000000e+00", "%e", 0.0);
  TEST_OUTPUT("0e+00", "%.0e", 0.0);
  TEST_OUTPUT("0.e+00", "%#.0e", 0.0);
  TEST_OUTPUT(" 0e+00", "% .0e", 0.0);
  TEST_OUTPUT("+0e+00", "%+.0e", 0.0);
}

TEST(snprintf, float10_e_pi) {
  TEST_OUTPUT("3e+00", "%.0e", M_PI);
  TEST_OUTPUT("3.1e+00", "%.1e", M_PI);
  TEST_OUTPUT("3.14e+00", "%.2e", M_PI);
  TEST_OUTPUT("3.142e+00", "%.3e", M_PI);
  TEST_OUTPUT("3.1416e+00", "%.4e", M_PI);
  TEST_OUTPUT("3.14159e+00", "%.5e", M_PI);
  TEST_OUTPUT("3.141593e+00", "%.6e", M_PI);
  TEST_OUTPUT("3.1415927e+00", "%.7e", M_PI);
  TEST_OUTPUT("3.14159265e+00", "%.8e", M_PI);

#if DBL_MANT_DIG == 53
  // This implementation already starts to return zeroes as soon as the
  // digits uniquely correspond with the floating point value.
  TEST_OUTPUT(
      "3."
      "141592653589793000000000000000000000000000000000000000000000000000000000"
      "0000000000000000000000000000e+00",
      "%.100e", M_PI);
#else
#error "Unknown floating point type"
#endif
}

TEST(snprintf, float16_nan) {
  TEST_OUTPUT("-nan      ", "%-10a", -NAN);
  TEST_OUTPUT("       NAN", "%10A", NAN);
}

TEST(snprintf, float16_inf) {
  TEST_OUTPUT("       inf", "%10a", INFINITY);
  TEST_OUTPUT("-INF      ", "%-10A", -INFINITY);
}

TEST(snprintf, float16_simple_zero) {
  TEST_OUTPUT("0x0p+0", "%a", 0.0);
  TEST_OUTPUT("0x0.p+0", "%#a", 0.0);
}

TEST(snprintf, float16_simple_one) {
  TEST_OUTPUT("0x1p+0", "%a", 1.0);
  TEST_OUTPUT("0x1.p+0", "%#a", 1.0);
}

TEST(snprintf, float16_simple_number) {
  TEST_OUTPUT("0x1.2345678p+24", "%a", 0x1234.5678p12);
}

TEST(snprintf, float16_negative) {
  TEST_OUTPUT("-0x1.2345678p+24", "%a", -0x1234.5678p12);
}

TEST(snprintf, float16_round) {
  ASSERT_EQ(0, fesetround(FE_DOWNWARD));
  TEST_OUTPUT("0x1.82p+0 -0x1.83p+0", "%.2a %.2a", 1.51, -1.51);
  ASSERT_EQ(0, fesetround(FE_TONEAREST));
  TEST_OUTPUT("0x1.83p+0 -0x1.83p+0", "%.2a %.2a", 1.51, -1.51);
  ASSERT_EQ(0, fesetround(FE_TOWARDZERO));
  TEST_OUTPUT("0x1.82p+0 -0x1.82p+0", "%.2a %.2a", 1.51, -1.51);
  ASSERT_EQ(0, fesetround(FE_UPWARD));
  TEST_OUTPUT("0x1.83p+0 -0x1.82p+0", "%.2a %.2a", 1.51, -1.51);
}

TEST(snprintf, float16_align) {
  TEST_OUTPUT("       0x1.2abc00000000000p+16", "%#30.15a", 0x12.abcp12);
  TEST_OUTPUT("0X1.2ABC00000000000P+16       ", "%#-30.15A", 0x12.abcp12);
  TEST_OUTPUT("0x00000001.2abc00000000000p+16", "%#030.15a", 0x12.abcp12);
  TEST_OUTPUT("+0x0000001.2abc00000000000p+16", "%+#030.15a", 0x12.abcp12);
  TEST_OUTPUT(" 0x0000001.2abc00000000000p+16", "% #030.15a", 0x12.abcp12);
}

#if LDBL_HAS_SUBNORM == 1
TEST(snprintf, float16_subnormal) {
#if LDBL_MANT_DIG == 53
  // Print subnormal values.
  TEST_OUTPUT("0x1p-1074", "%La", LDBL_TRUE_MIN);
  long double high = nexttowardl(LDBL_MIN, 0.0L);
  TEST_OUTPUT("0x1.ffffffffffffep-1023", "%La", high);

  // Test rounding behaviour of subnormal values.
  ASSERT_EQ(0, fesetround(FE_DOWNWARD));
  TEST_OUTPUT("0x1.ffp-1023 -0x1.00p-1022", "%.2La %.2La", high, -high);
  ASSERT_EQ(0, fesetround(FE_TONEAREST));
  TEST_OUTPUT("0x1.00p-1022 -0x1.00p-1022", "%.2La %.2La", high, -high);
  ASSERT_EQ(0, fesetround(FE_TOWARDZERO));
  TEST_OUTPUT("0x1.ffp-1023 -0x1.ffp-1023", "%.2La %.2La", high, -high);
  ASSERT_EQ(0, fesetround(FE_UPWARD));
  TEST_OUTPUT("0x1.00p-1022 -0x1.ffp-1023", "%.2La %.2La", high, -high);
#elif LDBL_MANT_DIG == 64
  // Print subnormal values.
  TEST_OUTPUT("0x1p-16445", "%La", LDBL_TRUE_MIN);
  long double high = nexttowardl(LDBL_MIN, 0.0L);
  TEST_OUTPUT("0x1.fffffffffffffffcp-16383", "%La", high);

  // Test rounding behaviour of subnormal values.
  ASSERT_EQ(0, fesetround(FE_DOWNWARD));
  TEST_OUTPUT("0x1.ffp-16383 -0x1.00p-16382", "%.2La %.2La", high, -high);
  ASSERT_EQ(0, fesetround(FE_TONEAREST));
  TEST_OUTPUT("0x1.00p-16382 -0x1.00p-16382", "%.2La %.2La", high, -high);
  ASSERT_EQ(0, fesetround(FE_TOWARDZERO));
  TEST_OUTPUT("0x1.ffp-16383 -0x1.ffp-16383", "%.2La %.2La", high, -high);
  ASSERT_EQ(0, fesetround(FE_UPWARD));
  TEST_OUTPUT("0x1.00p-16382 -0x1.ffp-16383", "%.2La %.2La", high, -high);
#elif LDBL_MANT_DIG == 113
  // Print subnormal values.
  TEST_OUTPUT("0x1p-16494", "0x1p-16494");
  long double high = nexttowardl(LDBL_MIN, 0.0L);
  TEST_OUTPUT("0x1.fffffffffffffffffffffffffffep-16383", "%La", high);

  // Test rounding behaviour of subnormal values.
  ASSERT_EQ(0, fesetround(FE_DOWNWARD));
  TEST_OUTPUT("0x1.ffp-16383 -0x1.00p-16382", "%.2La %.2La", high, -high);
  ASSERT_EQ(0, fesetround(FE_TONEAREST));
  TEST_OUTPUT("0x1.00p-16382 -0x1.00p-16382", "%.2La %.2La", high, -high);
  ASSERT_EQ(0, fesetround(FE_TOWARDZERO));
  TEST_OUTPUT("0x1.ffp-16383 -0x1.ffp-16383", "%.2La %.2La", high, -high);
  ASSERT_EQ(0, fesetround(FE_UPWARD));
  TEST_OUTPUT("0x1.00p-16382 -0x1.ffp-16383", "%.2La %.2La", high, -high);
#else
#error "Unknown floating point type"
#endif
}
#endif
