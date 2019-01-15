// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <string.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(strncat, example) {
  char buf[] = "\0AAAAAAAAA";
  ASSERT_EQ(buf, strncat(buf, "", 0));
  ASSERT_THAT(buf, testing::ElementsAreArray("\0AAAAAAAAA"));
  ASSERT_EQ(buf, strncat(buf, "Hello", 99999));
  ASSERT_THAT(buf, testing::ElementsAreArray("Hello\0AAAA"));
  ASSERT_EQ(buf, strncat(buf, "", 1));
  ASSERT_THAT(buf, testing::ElementsAreArray("Hello\0AAAA"));
  ASSERT_EQ(buf, strncat(buf, "!!!!!!!!!!!!", 3));
  ASSERT_THAT(buf, testing::ElementsAreArray("Hello!!!\0A"));
}
