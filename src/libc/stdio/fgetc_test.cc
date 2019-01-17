// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(fgetc, eagain) {
  // Create a stream to non-blocking read side of a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, fcntl(fds[0], F_SETFL, fcntl(fds[0], F_GETFL) | O_NONBLOCK));
  FILE *fp = fdopen(fds[0], "r");

  // Read cannot block.
  ASSERT_EQ(EOF, fgetc(fp));
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EAGAIN, errno);

  ASSERT_EQ(0, fclose(fp));
  ASSERT_EQ(0, close(fds[1]));
}

TEST(fgetc, ebadf) {
  // Create a stream to write side of a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  FILE *fp = fdopen(fds[1], "w");

  // Reading should not be permitted.
  ASSERT_EQ(EOF, fgetc(fp));
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EBADF, errno);

  ASSERT_EQ(0, fclose(fp));
  ASSERT_EQ(0, close(fds[0]));
}

TEST(fgetc, success) {
  // Create temporary file stream containing "ab".
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);
  ASSERT_LE(0, fputs("ab", fp));
  rewind(fp);

  // Read characters sequentially.
  ASSERT_EQ('a', fgetc(fp));
  ASSERT_FALSE(feof(fp));
  ASSERT_EQ('b', fgetc(fp));
  ASSERT_FALSE(feof(fp));
  ASSERT_EQ(EOF, fgetc(fp));
  ASSERT_TRUE(feof(fp));
  ASSERT_EQ(0, fclose(fp));
}
