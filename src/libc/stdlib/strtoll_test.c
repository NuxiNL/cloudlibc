// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <testing.h>

TEST(strtoll, positive) {
  const char *str;
  char *endptr;

  str = "0x7ffffffffffffffe";
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
