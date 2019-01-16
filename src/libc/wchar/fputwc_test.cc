// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <locale.h>
#include <stdio.h>
#include <unistd.h>
#include <wchar.h>

#include "gtest/gtest.h"

TEST(fputwc, eagain) {
  // Create a stream to non-blocking write side of a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, fcntl(fds[1], F_SETFL, fcntl(fds[1], F_GETFL) | O_NONBLOCK));
  FILE *fp = fdopen(fds[1], "w");

  // Write data into the pipe until it fails.
  wint_t ret;
  for (;;) {
    ret = fputwc(L'a', fp);
    if (ret == WEOF)
      break;
    ASSERT_EQ(L'a', ret);
    ASSERT_FALSE(ferror(fp));
  }

  // Final write should have failed with EAGAIN.
  ASSERT_EQ(WEOF, ret);
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EAGAIN, errno);

  // Closing should try to flush the data again, but it will still fail
  // with EAGAIN.
  errno = 0;
  ASSERT_EQ(EOF, fclose(fp));
  ASSERT_EQ(EAGAIN, errno);
  ASSERT_EQ(0, close(fds[0]));
}

TEST(fputwc, ebadf) {
  // Create a stream to the read side of a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  FILE *fp = fdopen(fds[0], "r");
  ASSERT_NE(NULL, fp);

  // Writing to the read side should fail.
  ASSERT_EQ(WEOF, fputwc(L'a', fp));
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EBADF, errno);

  ASSERT_EQ(0, fclose(fp));
  ASSERT_EQ(0, close(fds[1]));
}

TEST(fputwc, eilseq) {
  // Create an ASCII oriented temporary file.
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);

  // Writing a Unicode character should fail.
  ASSERT_EQ(WEOF, fputwc(L'☃', fp));
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EILSEQ, errno);

  ASSERT_EQ(0, fclose(fp));
}

TEST(fputwc, enospc) {
  // Create a very small Unicode capable temporary file.
  FILE *fp = fmemopen_l(NULL, 1, "w+", LC_C_UNICODE_LOCALE);
  ASSERT_NE(NULL, fp);

  // Writing a large Unicode character shall fail.
  ASSERT_EQ(WEOF, fputwc(L'☃', fp));
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(ENOSPC, errno);

  ASSERT_EQ(0, fclose(fp));
}

TEST(fputwc, epipe) {
  // Create a stream to non-blocking write side of a pipe that has
  // already been closed.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  FILE *fp = fdopen(fds[1], "w");
  ASSERT_EQ(0, close(fds[0]));

  // Write data into the pipe until it fails.
  wint_t ret;
  for (;;) {
    ret = fputwc(L'a', fp);
    if (ret == WEOF)
      break;
    ASSERT_EQ(L'a', ret);
    ASSERT_FALSE(ferror(fp));
  }

  // Final write should have failed with EPIPE.
  ASSERT_EQ(WEOF, ret);
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EPIPE, errno);

  // Closing should try to flush the data again, but it will still fail
  // with EPIPE.
  errno = 0;
  ASSERT_EQ(EOF, fclose(fp));
  ASSERT_EQ(EPIPE, errno);
}

TEST(fputwc, success) {
  // Create a large enough Unicode capable temporary file.
  FILE *fp = fmemopen_l(NULL, 9, "w+", LC_C_UNICODE_LOCALE);
  ASSERT_NE(NULL, fp);

  // Fill file with Unicode characters.
  ASSERT_EQ(L'☃', fputwc(L'☃', fp));
  ASSERT_EQ(L'☃', fputwc(L'☃', fp));
  ASSERT_EQ(L'☃', fputwc(L'☃', fp));
  ASSERT_FALSE(ferror(fp));

  // File should contain three consecutive snowmen.
  rewind(fp);
  char buf[10];
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp));
  ASSERT_STREQ("☃☃☃", buf);
  ASSERT_EQ(0, fclose(fp));
}
