// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdio.h>

#include "gtest/gtest.h"

TEST(fswap, example) {
  // Prepare two streams containing "Hello" and "World".
  FILE *fp1 = tmpfile();
  ASSERT_NE(NULL, fp1);
  ASSERT_LE(0, fputs("Hello", fp1));
  rewind(fp1);
  FILE *fp2 = tmpfile();
  ASSERT_NE(NULL, fp2);
  ASSERT_LE(0, fputs("World", fp2));
  rewind(fp2);

  // Swap them around.
  fswap(fp1, fp2);

  // Stream contents should have been toggled.
  char buf[6];
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp1));
  ASSERT_STREQ("World", buf);
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp2));
  ASSERT_STREQ("Hello", buf);

  ASSERT_EQ(0, fclose(fp1));
  ASSERT_EQ(0, fclose(fp2));
}
