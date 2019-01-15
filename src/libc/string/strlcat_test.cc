// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(strlcat, null) {
  ASSERT_EQ(5, strlcat(NULL, "Hello", 0));
}

TEST(strlcat, one) {
  char buf = '\0';
  ASSERT_EQ(6, strlcat(&buf, "Banana", 1));
  ASSERT_EQ('\0', buf);

  buf = 'A';
  ASSERT_EQ(7, strlcat(&buf, "Banana", 1));
  ASSERT_EQ('A', buf);
}

TEST(strlcat, longer) {
  char buf[] = "AAAAAAAAAAAA";
  ASSERT_EQ(15, strlcat(buf, "Foo", sizeof(buf) - 1));
  ASSERT_THAT(buf, testing::ElementsAreArray("AAAAAAAAAAAA"));

  buf[4] = '\0';
  ASSERT_EQ(7, strlcat(buf, "Bar", sizeof(buf) - 1));
  ASSERT_THAT(buf, testing::ElementsAreArray("AAAABar\0AAAA"));

  ASSERT_EQ(16, strlcat(buf, "Very long", sizeof(buf) - 1));
  ASSERT_THAT(buf, testing::ElementsAreArray("AAAABarVery\0"));
}
