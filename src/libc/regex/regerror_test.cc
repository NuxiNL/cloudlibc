// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <regex.h>
#include <stddef.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(regerror, example) {
  ASSERT_EQ(8, regerror(0, NULL, NULL, 0));

  char buf[] = "AAAAAAAAA";
  ASSERT_EQ(8, regerror(0, NULL, buf, 0));
  ASSERT_THAT(buf, testing::ElementsAreArray("AAAAAAAAA"));

  ASSERT_EQ(8, regerror(0, NULL, buf, 1));
  ASSERT_THAT(buf, testing::ElementsAreArray("\0AAAAAAAA"));

  ASSERT_EQ(8, regerror(0, NULL, buf, 5));
  ASSERT_THAT(buf, testing::ElementsAreArray("Succ\0AAAA"));

  ASSERT_EQ(8, regerror(0, NULL, buf, 9));
  ASSERT_THAT(buf, testing::ElementsAreArray("Success\0A"));
}
