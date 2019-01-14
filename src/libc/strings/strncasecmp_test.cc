// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <strings.h>

#include "gtest/gtest.h"

// TODO(ed): Add tests for invalid encodings.

TEST(strncasecmp, null) {
  ASSERT_EQ(0, strncasecmp(NULL, NULL, 0));
}

TEST(strncasecmp, example) {
  ASSERT_EQ(0, strncasecmp("hello", "hello", 100));
  ASSERT_EQ(0, strncasecmp("hElLo", "hello", 100));

  ASSERT_EQ(0, strncasecmp("doge", "dogS", 3));
  ASSERT_GT(0, strncasecmp("doge", "dogS", 4));
  ASSERT_EQ(0, strncasecmp("dogs", "dogE", 3));
  ASSERT_LT(0, strncasecmp("dogs", "dogE", 4));
}

TEST(strncasecmp, unicode) {
  ASSERT_EQ(0, strncasecmp_l("München?", "MÜNCHEN!", 8, LC_C_UNICODE_LOCALE));
  ASSERT_LT(0, strncasecmp_l("München?", "MÜNCHEN!", 9, LC_C_UNICODE_LOCALE));
}
