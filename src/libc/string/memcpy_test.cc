// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(memcpy, null) {
  ASSERT_EQ((char *)42, memcpy((char *)42, (char *)123, 0));
}

TEST(memcpy, example) {
  const char buf1[8] = "Foo\0Bar";
  char buf2[8];
  ASSERT_EQ(buf2, memcpy(buf2, buf1, sizeof(buf1)));
  ASSERT_THAT(buf2, testing::ElementsAreArray(buf1));
}
