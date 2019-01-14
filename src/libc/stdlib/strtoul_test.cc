// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(strtoul, examples) {
  const char *str = "  57";
  char *endptr;
  errno = 0;
  ASSERT_EQ(57, strtoul(str, NULL, 10));
  ASSERT_EQ(0, errno);

  str = "          ";
  ASSERT_EQ(0, strtoul(str, &endptr, 10));
  ASSERT_EQ(str, endptr);
  ASSERT_EQ(EINVAL, errno);

  str = "  01234hello";
  errno = 0;
  ASSERT_EQ(1234, strtoul(str, &endptr, 10));
  ASSERT_EQ(str + 7, endptr);
  ASSERT_EQ(0, errno);

  str = "  01234hello";
  ASSERT_EQ(194, strtoul(str, &endptr, 5));
  ASSERT_EQ(str + 7, endptr);
  ASSERT_EQ(0, errno);

  str = "  01234hello";
  ASSERT_EQ(01234, strtoul(str, &endptr, 0));
  ASSERT_EQ(str + 7, endptr);
  ASSERT_EQ(0, errno);

  str = "Hello!";
  ASSERT_EQ(29234652, strtoul(str, &endptr, 36));
  ASSERT_EQ(str + 5, endptr);
  ASSERT_EQ(0, errno);

  str = "\n-42boom";
  ASSERT_EQ(ULONG_MAX, strtoul(str, &endptr, 6));
  ASSERT_EQ(str + 4, endptr);
  ASSERT_EQ(ERANGE, errno);

  str = "\t-000000";
  errno = 0;
  ASSERT_EQ(0, strtoul(str, &endptr, 6));
  ASSERT_EQ(str + 8, endptr);
  ASSERT_EQ(0, errno);

  str = "0x123";
  ASSERT_EQ(0x123, strtoul(str, &endptr, 0));
  ASSERT_EQ(str + 5, endptr);
  ASSERT_EQ(0, errno);

  str = "456";
  ASSERT_EQ(0x456, strtoul(str, &endptr, 16));
  ASSERT_EQ(str + 3, endptr);
  ASSERT_EQ(0, errno);
}

TEST(strtoul, unicode) {
  // U+3000: Ideographic space. Should not be matched with the C locale.
  const char *str =
      "\xe3\x80\x80"
      "123";
  char *endptr;
  ASSERT_EQ(0, strtoul(str, &endptr, 10));
  ASSERT_EQ(str, endptr);
  ASSERT_EQ(EINVAL, errno);

  errno = 0;
  ASSERT_EQ(123, strtoul_l(str, &endptr, 10, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(str + 6, endptr);
  ASSERT_EQ(0, errno);
}
