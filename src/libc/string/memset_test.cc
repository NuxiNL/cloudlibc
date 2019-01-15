// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(memset, null) {
  ASSERT_EQ((char *)5, memset((char *)5, 'A', 0));
}

TEST(memset, example_small) {
  char buf[11];
  ASSERT_EQ(buf, memset(buf, '!', 10));
  buf[10] = '\0';
  ASSERT_STREQ("!!!!!!!!!!", buf);
}

TEST(memset, example_large) {
  char buf[101];
  ASSERT_EQ(buf, memset(buf, '!', 100));
  buf[100] = '\0';
  ASSERT_THAT(buf,
              testing::ElementsAreArray("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
                                        "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
                                        "!!!!!!!!!!!!!!!!!!!!!!!!"));
}
