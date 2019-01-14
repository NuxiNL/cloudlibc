// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(strtoull, positive) {
  const char *str;
  char *endptr;

  errno = 0;
  str = "0xfffffffffffffffe";
  ASSERT_EQ(ULLONG_MAX - 1, strtoull(str, &endptr, 0));
  ASSERT_EQ(str + 18, endptr);
  ASSERT_EQ(0, errno);

  str = "0xffffffffffffffff";
  ASSERT_EQ(ULLONG_MAX, strtoull(str, &endptr, 0));
  ASSERT_EQ(str + 18, endptr);
  ASSERT_EQ(0, errno);

  str = "0x10000000000000000";
  ASSERT_EQ(ULLONG_MAX, strtoull(str, &endptr, 0));
  ASSERT_EQ(str + 19, endptr);
  ASSERT_EQ(ERANGE, errno);

  str = "0xfffffffffffffffff";
  errno = 0;
  ASSERT_EQ(ULLONG_MAX, strtoull(str, &endptr, 0));
  ASSERT_EQ(str + 19, endptr);
  ASSERT_EQ(ERANGE, errno);
}

TEST(strtoull, negative) {
  const char *str;
  char *endptr;

  errno = 0;
  str = "0";
  ASSERT_EQ(0, strtoull(str, &endptr, 0));
  ASSERT_EQ(str + 1, endptr);
  ASSERT_EQ(0, errno);

  str = "-0";
  ASSERT_EQ(0, strtoull(str, &endptr, 0));
  ASSERT_EQ(str + 2, endptr);
  ASSERT_EQ(0, errno);

  str = "-1";
  ASSERT_EQ(ULLONG_MAX, strtoull(str, &endptr, 0));
  ASSERT_EQ(str + 2, endptr);
  ASSERT_EQ(ERANGE, errno);
}
