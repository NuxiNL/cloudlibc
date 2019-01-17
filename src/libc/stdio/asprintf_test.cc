// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(asprintf, hello) {
  char *copy;
  ASSERT_EQ(11, asprintf(&copy, "%d + %d = %d", 13, 7, 20));
  ASSERT_STREQ("13 + 7 = 20", copy);
  free(copy);
}

TEST(asprintf, eilseq) {
  ASSERT_EQ(-1, asprintf(NULL, "%ls\n", L"Hawaï"));
  ASSERT_EQ(EILSEQ, errno);
}

TEST(asprintf, unicode) {
  char *copy;
  ASSERT_EQ(7, asprintf_l(&copy, LC_C_UNICODE_LOCALE, "%ls\n", L"Hawaï"));
  ASSERT_STREQ("Hawaï\n", copy);
  free(copy);
}
