// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <unistd.h>
#include <wchar.h>

#include "gtest/gtest.h"

TEST(ungetwc, eilseq) {
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);

  // Attempted to push a Unicode character into an ASCII stream.
  ASSERT_EQ(WEOF, ungetwc(L'☃', fp));
  ASSERT_FALSE(ferror(fp));
  ASSERT_EQ(EILSEQ, errno);

  ASSERT_EQ(0, fclose(fp));
}

TEST(ungetwc, einval) {
  // Pushing WEOF should leave the stream unchanged.
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);
  ASSERT_EQ(WEOF, ungetwc(WEOF, fp));
  ASSERT_EQ(EINVAL, errno);
  ASSERT_EQ(0, fclose(fp));
}

TEST(ungetwc, enospc) {
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);

  // Push back characters until it fails.
  wint_t ret;
  for (;;) {
    ret = ungetwc(L'a', fp);
    if (ret == WEOF)
      break;
    ASSERT_EQ(L'a', ret);
  }

  // Final push should have failed with ENOSPC.
  ASSERT_EQ(WEOF, ret);
  ASSERT_FALSE(ferror(fp));
  ASSERT_EQ(ENOSPC, errno);

  ASSERT_EQ(0, fclose(fp));
}

TEST(ungetwc, success) {
  // Create a Unicode capable stream containing a Unicode snowman.
  FILE *fp = tmpfile_l(LC_C_UNICODE_LOCALE);
  ASSERT_NE(NULL, fp);
  ASSERT_EQ(L'☃', ungetwc(L'☃', fp));

  // Character should be decomposed into UTF-8.
  char buf[4];
  ASSERT_EQ(3, fread(buf, 1, sizeof(buf), fp));
  buf[3] = '\0';
  ASSERT_STREQ("☃", buf);

  ASSERT_EQ(0, fclose(fp));
}
