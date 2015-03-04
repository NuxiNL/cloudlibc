// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/float.h>

#include <limits.h>
#include <testing.h>

static void test_number(uint64_t high, uint64_t low, int exponent,
                        float value) {
  struct float2 f2 = {
      .significand = {high, low}, .exponent = exponent,
  };
  float f;
  ASSERT_TRUE(__float2_to_float(&f2, &f));
  ASSERT_EQ(value, f);
}

TEST(float2_to_float_test, examples) {
  // Zero significand should always return zero, regardless of the exponent.
  test_number(0x0000000000000000, 0x0000000000000000, INT_MIN, 0.0);
  test_number(0x0000000000000000, 0x0000000000000000, -127, 0.0);
  test_number(0x0000000000000000, 0x0000000000000000, INT_MAX, 0.0);

  test_number(0x8000000000000000, 0x0000000000000000, -127, 1.0);
  test_number(0xc000000000000000, 0x0000000000000000, -127, 1.5);
  test_number(0x8000000000000000, 0x0000000000000000, -126, 2.0);
  test_number(0xc000000000000000, 0x0000000000000000, -126, 3.0);
}
