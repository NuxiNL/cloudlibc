// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdio.h>
#include <testing.h>

TEST(ferror, example) {
  // Error indicator should be clear by default.
  FILE *fp = fmemopen(NULL, 10, "w+");
  ASSERT_NE(NULL, fp);
  ASSERT_FALSE(ferror(fp));

  // Writing beyond the end of the buffer should set it.
  fprintf(fp, "Hello, world");
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(0, fclose(fp));
}
