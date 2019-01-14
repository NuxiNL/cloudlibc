// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <strings.h>

#include "gtest/gtest.h"

// TODO(ed): Add tests for invalid encodings.

TEST(strcasecmp, example) {
  ASSERT_EQ(0, strcasecmp("hello", "hello"));
  ASSERT_EQ(0, strcasecmp("hElLo", "hello"));

  ASSERT_GT(0, strcasecmp("doge", "dogS"));
  ASSERT_LT(0, strcasecmp("dogs", "dogE"));
}

TEST(strcasecmp, unicode) {
  ASSERT_LT(0, strcasecmp_l("München?", "MÜNCHEN!", LC_C_UNICODE_LOCALE));
  ASSERT_EQ(0, strcasecmp_l("München", "MÜNCHEN", LC_C_UNICODE_LOCALE));
}
