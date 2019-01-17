// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdio.h>

#include "gtest/gtest.h"

TEST(clearerr, error) {
  // Create memory file of 10 bytes.
  char buf[11];
  FILE *file = fmemopen(buf, sizeof(buf) - 1, "w");
  ASSERT_NE(NULL, file);
  ASSERT_FALSE(feof(file));
  ASSERT_FALSE(ferror(file));

  // We can only write up to five groups of two letters.
  ASSERT_EQ(5, fwrite("Hello, world", 2, 6, file));
  buf[10] = '\0';
  ASSERT_STREQ("Hello, wor", buf);

  // Clear error condition.
  ASSERT_FALSE(feof(file));
  ASSERT_TRUE(ferror(file));
  clearerr(file);
  ASSERT_FALSE(feof(file));
  ASSERT_FALSE(ferror(file));

  ASSERT_EQ(0, fclose(file));
}

TEST(clearerr, eof) {
  // Create memory file of 12 bytes.
  const char buf[] = "Hello, world";
  FILE *file = fmemopen((void *)buf, sizeof(buf) - 1, "r");
  ASSERT_NE(NULL, file);
  ASSERT_FALSE(feof(file));
  ASSERT_FALSE(ferror(file));

  // We can only read up to two groups of five letters.
  char readbuf[15];
  ASSERT_EQ(2, fread(readbuf, 5, 3, file));
  readbuf[10] = '\0';
  ASSERT_STREQ("Hello, wor", readbuf);

  // Clear error condition.
  ASSERT_TRUE(feof(file));
  ASSERT_FALSE(ferror(file));
  clearerr(file);
  ASSERT_FALSE(feof(file));
  ASSERT_FALSE(ferror(file));

  ASSERT_EQ(0, fclose(file));
}
