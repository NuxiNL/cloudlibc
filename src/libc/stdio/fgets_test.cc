// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(fgets, eagain) {
  // Create a stream to non-blocking read side of a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, fcntl(fds[0], F_SETFL, fcntl(fds[0], F_GETFL) | O_NONBLOCK));
  FILE *fp = fdopen(fds[0], "r");

  // Read cannot block.
  char buf[10];
  ASSERT_EQ(NULL, fgets(buf, sizeof(buf), fp));
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EAGAIN, errno);

  ASSERT_EQ(0, fclose(fp));
  ASSERT_EQ(0, close(fds[1]));
}

TEST(fgets, ebadf) {
  // Create a stream to write side of a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  FILE *fp = fdopen(fds[1], "w");

  // Reading should not be permitted.
  char buf[10];
  ASSERT_EQ(NULL, fgets(buf, sizeof(buf), fp));
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EBADF, errno);

  ASSERT_EQ(0, fclose(fp));
  ASSERT_EQ(0, close(fds[0]));
}

TEST(fgets, small) {
  // Non-positive size should just return a NULL pointer, regardless of
  // the stream provided.
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);
  ASSERT_EQ(NULL, fgets(NULL, -123, fp));
  ASSERT_EQ(NULL, fgets(NULL, 0, fp));

  // One-sized buffer always returns an empty string, regardless of the
  // stream provided.
  char buf[1];
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp));
  ASSERT_STREQ("", buf);

  // Stream should still be in the initial state.
  ASSERT_EQ(0, ftello(fp));
  ASSERT_FALSE(feof(fp));
  ASSERT_FALSE(ferror(fp));
  ASSERT_EQ(0, fclose(fp));
}

TEST(fgets, example) {
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);

  // Read lines from a sample file.
  ASSERT_LE(0, fputs("some\nsentences\nin\n\na\nfile", fp));
  rewind(fp);
  char buf[8];
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp));
  ASSERT_STREQ("some\n", buf);
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp));
  ASSERT_STREQ("sentenc", buf);
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp));
  ASSERT_STREQ("es\n", buf);
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp));
  ASSERT_STREQ("in\n", buf);
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp));
  ASSERT_STREQ("\n", buf);
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp));
  ASSERT_STREQ("a\n", buf);
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp));
  ASSERT_STREQ("file", buf);
  ASSERT_TRUE(feof(fp));
  ASSERT_EQ(NULL, fgets(buf, sizeof(buf), fp));
  ASSERT_TRUE(feof(fp));

  // Repeat the process where the final line has a newline.
  ASSERT_EQ('\n', putc('\n', fp));
  rewind(fp);
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp));
  ASSERT_STREQ("some\n", buf);
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp));
  ASSERT_STREQ("sentenc", buf);
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp));
  ASSERT_STREQ("es\n", buf);
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp));
  ASSERT_STREQ("in\n", buf);
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp));
  ASSERT_STREQ("\n", buf);
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp));
  ASSERT_STREQ("a\n", buf);
  ASSERT_EQ(buf, fgets(buf, sizeof(buf), fp));
  ASSERT_STREQ("file\n", buf);
  ASSERT_FALSE(feof(fp));
  ASSERT_EQ(NULL, fgets(buf, sizeof(buf), fp));
  ASSERT_TRUE(feof(fp));

  ASSERT_EQ(0, fclose(fp));
}
