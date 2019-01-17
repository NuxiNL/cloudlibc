// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(getdelim, example) {
  static const char mytext[] = "This is a string that we are going to split";
  FILE *file = fmemopen((char *)mytext, sizeof(mytext) - 1, "r");
  char *lineptr = NULL;
  size_t n;

  ASSERT_EQ(-1, getdelim(NULL, &n, ' ', file));
  ASSERT_TRUE(ferror(file));
  ASSERT_EQ(EINVAL, errno);
  clearerr(file);
  ASSERT_EQ(-1, getdelim(&lineptr, NULL, ' ', file));
  ASSERT_TRUE(ferror(file));
  ASSERT_EQ(EINVAL, errno);
  clearerr(file);
  ASSERT_EQ(-1, getdelim(NULL, NULL, ' ', file));
  ASSERT_TRUE(ferror(file));
  ASSERT_EQ(EINVAL, errno);

  ASSERT_EQ(5, getdelim(&lineptr, &n, ' ', file));
  ASSERT_EQ(16, n);
  ASSERT_STREQ("This ", lineptr);
  ASSERT_FALSE(feof(file));

  ASSERT_EQ(1, getdelim(&lineptr, &n, 'i', file));
  ASSERT_EQ(16, n);
  ASSERT_STREQ("i", lineptr);
  ASSERT_FALSE(feof(file));

  ASSERT_EQ(25, getdelim(&lineptr, &n, 'o', file));
  ASSERT_EQ(32, n);
  ASSERT_STREQ("s a string that we are go", lineptr);
  ASSERT_FALSE(feof(file));

  ASSERT_EQ(12, getline(&lineptr, &n, file));
  ASSERT_EQ(32, n);
  ASSERT_STREQ("ing to split", lineptr);
  ASSERT_TRUE(feof(file));

  ASSERT_EQ(-1, getline(&lineptr, &n, file));
  ASSERT_TRUE(feof(file));

  ASSERT_EQ(0, fclose(file));
  free(lineptr);
}
