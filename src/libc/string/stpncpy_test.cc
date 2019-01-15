// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(stpncpy, null) {
  ASSERT_EQ((char *)12, stpncpy((char *)12, (char *)500, 0));
}

TEST(stpncpy, example1) {
  char buf[] = "AAAAAAAAAAAA";
  ASSERT_EQ(buf + 5, stpncpy(buf, "Hello", 12));
  ASSERT_THAT(buf, testing::ElementsAreArray("Hello\0\0\0\0\0\0\0"));
}

TEST(stpncpy, example2) {
  char buf[] = "AAAAAAAAAAAA";
  ASSERT_EQ(buf + 12, stpncpy(buf, "This is a very long string", 12));
  ASSERT_THAT(buf, testing::ElementsAreArray("This is a ve"));
}
