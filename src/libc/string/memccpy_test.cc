// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(memccpy, null) {
  ASSERT_EQ(NULL, memccpy((char *)456, (char *)789, 'A', 0));
}

TEST(memccpy, example) {
  const char buf1[13] = "Test\0string!";
  char buf2[] = "AAAAAAAAA";
  ASSERT_EQ(&buf2[8], memccpy(buf2, buf1, 'r', 9999));
  ASSERT_THAT(buf2, testing::ElementsAreArray("Test\0strA"));
}
