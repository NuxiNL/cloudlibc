// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdio.h>

#include "gtest/gtest.h"

TEST(feof, example) {
  // End-of-file indicator should be clear by default.
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);
  ASSERT_FALSE(feof(fp));

  // Reading a character at the end of the file should set it.
  ASSERT_EQ(EOF, getc(fp));
  ASSERT_TRUE(feof(fp));
  ASSERT_EQ(0, fclose(fp));
}
