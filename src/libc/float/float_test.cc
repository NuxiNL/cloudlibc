// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <float.h>
#include <math.h>

#include "gtest/gtest.h"

TEST(float, mant_dig) {
  ASSERT_EQ(FLT_RADIX, scalbnf(FLT_EPSILON, FLT_MANT_DIG));
  ASSERT_EQ(FLT_RADIX, scalbn(DBL_EPSILON, DBL_MANT_DIG));
  ASSERT_EQ(FLT_RADIX, scalbnl(LDBL_EPSILON, LDBL_MANT_DIG));
}

TEST(float, true_min) {
  // If we support subnormal numbers, we can go down by another factor
  // FLT_EPSILON.
  ASSERT_TRUE(!FLT_HAS_SUBNORM || FLT_TRUE_MIN == FLT_MIN * FLT_EPSILON);
  ASSERT_TRUE(!DBL_HAS_SUBNORM || DBL_TRUE_MIN == DBL_MIN * DBL_EPSILON);
  ASSERT_TRUE(!LDBL_HAS_SUBNORM || LDBL_TRUE_MIN == LDBL_MIN * LDBL_EPSILON);
}

TEST(float, max) {
  ASSERT_FALSE(isinf(FLT_MAX * 1));
  ASSERT_FALSE(isinf(DBL_MAX * 1));
  ASSERT_FALSE(isinf(LDBL_MAX * 1));

  ASSERT_TRUE(isinf(FLT_MAX * FLT_RADIX));
  ASSERT_TRUE(isinf(DBL_MAX * FLT_RADIX));
  ASSERT_TRUE(isinf(LDBL_MAX * FLT_RADIX));
}

// Test whether FLT_EVAL_METHOD is in sync with float_t and double_t.
#define TEST_TYPE(a, b) _Generic((a)0, b : 1, default : 0)
static_assert(FLT_EVAL_METHOD >= -1, "Value mismatch");
static_assert(FLT_EVAL_METHOD <= 2, "Value mismatch");
static_assert(FLT_EVAL_METHOD != 0 ||
                  (TEST_TYPE(float_t, float) && TEST_TYPE(double_t, double)),
              "float_t and double_t incorrectly defined");
static_assert(FLT_EVAL_METHOD != 1 ||
                  (TEST_TYPE(float_t, double) && TEST_TYPE(double_t, double)),
              "float_t and double_t incorrectly defined");
static_assert(FLT_EVAL_METHOD != 2 || (TEST_TYPE(float_t, long double) &&
                                       TEST_TYPE(double_t, long double)),
              "float_t and double_t incorrectly defined");
#undef TEST_TYPE

static_assert(FLT_RADIX >= 2, "Value mismatch");

#if FLT_RADIX == 2
// Rough approximations for exponents.
static_assert(FLT_MIN_10_EXP <= FLT_MIN_EXP / 4, "Value mismatch");
static_assert(FLT_MIN_10_EXP >= FLT_MIN_EXP / 3, "Value mismatch");
static_assert(FLT_MAX_10_EXP >= FLT_MAX_EXP / 4, "Value mismatch");
static_assert(FLT_MAX_10_EXP <= FLT_MAX_EXP / 3, "Value mismatch");

static_assert(DBL_MIN_10_EXP <= DBL_MIN_EXP / 4, "Value mismatch");
static_assert(DBL_MIN_10_EXP >= DBL_MIN_EXP / 3, "Value mismatch");
static_assert(DBL_MAX_10_EXP >= DBL_MAX_EXP / 4, "Value mismatch");
static_assert(DBL_MAX_10_EXP <= DBL_MAX_EXP / 3, "Value mismatch");

static_assert(LDBL_MIN_10_EXP <= LDBL_MIN_EXP / 4, "Value mismatch");
static_assert(LDBL_MIN_10_EXP >= LDBL_MIN_EXP / 3, "Value mismatch");
static_assert(LDBL_MAX_10_EXP >= LDBL_MAX_EXP / 4, "Value mismatch");
static_assert(LDBL_MAX_10_EXP <= LDBL_MAX_EXP / 3, "Value mismatch");

// Rough approximations for the number of digits.
static_assert(FLT_DIG >= FLT_MANT_DIG / 4, "Value mismatch");
static_assert(FLT_DIG <= FLT_MANT_DIG / 3, "Value mismatch");

static_assert(DBL_DIG >= DBL_MANT_DIG / 4, "Value mismatch");
static_assert(DBL_DIG <= DBL_MANT_DIG / 3, "Value mismatch");

static_assert(LDBL_DIG >= LDBL_MANT_DIG / 4, "Value mismatch");
static_assert(LDBL_DIG <= LDBL_MANT_DIG / 3, "Value mismatch");
#endif

// Number of decimal digits should match up with long double's number of
// decimal digits, as it is the largest.
static_assert(DECIMAL_DIG >= FLT_DECIMAL_DIG, "Value mismatch");
static_assert(DECIMAL_DIG >= DBL_DECIMAL_DIG, "Value mismatch");
static_assert(DECIMAL_DIG == LDBL_DECIMAL_DIG, "Value mismatch");
