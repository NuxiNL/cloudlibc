// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(strncpy, null) {
  ASSERT_EQ((char *)12, strncpy((char *)12, (char *)500, 0));
}

TEST(strncpy, example1) {
  char buf[] = "AAAAAAAAAAAA";
  ASSERT_EQ(buf, strncpy(buf, "Hello", 12));
  ASSERT_THAT(buf, testing::ElementsAreArray("Hello\0\0\0\0\0\0\0"));
}

TEST(strncpy, example2) {
  char buf[13];
  ASSERT_EQ(buf, strncpy(buf, "This is a very long string", 12));
  buf[12] = '\0';
  ASSERT_THAT(buf, testing::ElementsAreArray("This is a ve"));
}
