// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(fread, eagain) {
  // Create a stream to non-blocking read side of a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, fcntl(fds[0], F_SETFL, fcntl(fds[0], F_GETFL) | O_NONBLOCK));
  FILE *fp = fdopen(fds[0], "r");

  // Read cannot block.
  char buf[1];
  ASSERT_EQ(0, fread(buf, 1, sizeof(buf), fp));
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EAGAIN, errno);

  ASSERT_EQ(0, fclose(fp));
  ASSERT_EQ(0, close(fds[1]));
}

TEST(fread, ebadf) {
  // Create a stream to write side of a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  FILE *fp = fdopen(fds[1], "w");

  // Reading should not be permitted.
  char buf[1];
  ASSERT_EQ(0, fread(buf, 1, sizeof(buf), fp));
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EBADF, errno);

  ASSERT_EQ(0, fclose(fp));
  ASSERT_EQ(0, close(fds[0]));
}

TEST(fread, einval) {
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);

  // Extension: catch multiplication overflow. This is also done on
  // other systems like FreeBSD.
  ASSERT_EQ(0, fread(NULL, SIZE_MAX, SIZE_MAX, fp));
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EINVAL, errno);

  ASSERT_EQ(0, fclose(fp));
}

TEST(fread, zero) {
  // Zero-sized reads should leave the stream unaffected.
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);
  ASSERT_EQ(0, fread(NULL, 12308120, 0, fp));
  ASSERT_EQ(0, fread(NULL, 0, 5492142, fp));
  ASSERT_EQ(0, fread(NULL, 0, 0, fp));
  ASSERT_EQ(0, ftello(fp));
  ASSERT_FALSE(feof(fp));
  ASSERT_FALSE(ferror(fp));
  ASSERT_EQ(0, fclose(fp));
}

TEST(fread, example) {
  FILE *fp = fmemopen((char *)"HelloWorld", 10, "r");
  ASSERT_NE(NULL, fp);

  // Perform reads using different object sizes.
  for (size_t i = 1; i <= 11; ++i) {
    char buf[12] = {};
    ASSERT_EQ(10 / i, fread(buf, i, sizeof(buf) / i, fp));
    size_t total_length = 10 / i * i;
    buf[total_length] = '\0';
    ASSERT_THAT("HelloWorld", testing::StartsWith(buf));
    ASSERT_EQ(sizeof(buf) / i * i > 10, feof(fp));
    ASSERT_FALSE(ferror(fp));
    rewind(fp);
  }

  ASSERT_EQ(0, fclose(fp));
}
