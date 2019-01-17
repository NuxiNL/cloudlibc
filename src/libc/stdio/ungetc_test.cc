// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(ungetc, einval) {
  // Pushing EOF should leave the stream unchanged.
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);
  ASSERT_EQ(EOF, ungetc(EOF, fp));
  ASSERT_EQ(EINVAL, errno);
  ASSERT_EQ(0, fclose(fp));
}

TEST(ungetc, enospc) {
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);

  // Push back characters until it fails.
  int ret;
  for (;;) {
    ret = ungetc('a', fp);
    if (ret == EOF)
      break;
    ASSERT_EQ('a', ret);
  }

  // Final push should have failed with ENOSPC.
  ASSERT_EQ(EOF, ret);
  ASSERT_FALSE(ferror(fp));
  ASSERT_EQ(ENOSPC, errno);

  ASSERT_EQ(0, fclose(fp));
}

TEST(ungetc, success) {
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);
  ASSERT_EQ('a', ungetc('a', fp));
  ASSERT_EQ('a', getc(fp));
  ASSERT_EQ(EOF, getc(fp));
  ASSERT_EQ(0, fclose(fp));
}
