// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>

#include "gtest/gtest.h"

TEST(setlocale, good) {
  ASSERT_STREQ("C", setlocale(LC_ALL, NULL));
  ASSERT_STREQ("C", setlocale(LC_CTYPE, ""));
  ASSERT_STREQ("C", setlocale(LC_MESSAGES, "C"));
  ASSERT_STREQ("C", setlocale(LC_TIME, "POSIX"));
}

TEST(setlocale, bad) {
  ASSERT_EQ(NULL, setlocale(LC_ALL, "De"));
  ASSERT_EQ(NULL, setlocale(LC_COLLATE, "Fr@dict"));
}
