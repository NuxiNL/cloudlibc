// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "gtest/gtest.h"

TEST(strsep, example) {
  char line[] = "LINE\nTO BE\tSEPARATED\n";
  const char *split = " \t\n";
  char *lasts = line;
  ASSERT_STREQ("LINE", strsep(&lasts, split));
  ASSERT_STREQ("TO", strsep(&lasts, split));
  ASSERT_STREQ("BE", strsep(&lasts, split));
  ASSERT_STREQ("SEPARATED", strsep(&lasts, split));
  ASSERT_STREQ("", strsep(&lasts, split));
  ASSERT_EQ(NULL, strsep(&lasts, split));
}
