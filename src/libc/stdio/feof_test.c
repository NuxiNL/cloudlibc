// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdio.h>
#include <testing.h>

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
