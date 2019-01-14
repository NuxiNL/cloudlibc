// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(strtoll, positive) {
  const char *str;
  char *endptr;

  str = "0x7ffffffffffffffe";
  errno = 0;
  ASSERT_EQ(LLONG_MAX - 1, strtoll(str, &endptr, 0));
  ASSERT_EQ(str + 18, endptr);
  ASSERT_EQ(0, errno);

  str = "0x7fffffffffffffff";
  ASSERT_EQ(LLONG_MAX, strtoll(str, &endptr, 0));
  ASSERT_EQ(str + 18, endptr);
  ASSERT_EQ(0, errno);

  str = "0x8000000000000000";
  ASSERT_EQ(LLONG_MAX, strtoll(str, &endptr, 0));
  ASSERT_EQ(str + 18, endptr);
  ASSERT_EQ(ERANGE, errno);
}

TEST(strtoll, negative) {
  const char *str;
  char *endptr;

  str = "-0x7fffffffffffffff";
  errno = 0;
  ASSERT_EQ(LLONG_MIN + 1, strtoll(str, &endptr, 0));
  ASSERT_EQ(str + 19, endptr);
  ASSERT_EQ(0, errno);

  str = "-0x8000000000000000";
  ASSERT_EQ(LLONG_MIN, strtoll(str, &endptr, 0));
  ASSERT_EQ(str + 19, endptr);
  ASSERT_EQ(0, errno);

  str = "-0x8000000000000001";
  ASSERT_EQ(LLONG_MIN, strtoll(str, &endptr, 0));
  ASSERT_EQ(str + 19, endptr);
  ASSERT_EQ(ERANGE, errno);
}
