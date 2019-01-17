// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(fseeko, ebadf) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Create a stream of which the underlying descriptor is closed.
  FILE *fp = fopenat(fd_tmp, "hello", "w");
  ASSERT_NE(NULL, fp);
  ASSERT_EQ(0, close(fileno(fp)));

  // Seeking with data in the write buffer should fail.
  ASSERT_LE(0, fputs("Hello", fp));
  ASSERT_EQ(-1, fseeko(fp, 0, SEEK_SET));
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EBADF, errno);

  ASSERT_EQ(EOF, fclose(fp));
}

TEST(fseeko, einval) {
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);

  // Bad whence argument.
  ASSERT_EQ(-1, fseeko(fp, 0, 0xdeadc0de));
  ASSERT_FALSE(ferror(fp));
  ASSERT_EQ(EINVAL, errno);

  // Negative file position.
  ASSERT_EQ(-1, fseeko(fp, -10, SEEK_SET));
  ASSERT_FALSE(ferror(fp));
  ASSERT_EQ(EINVAL, errno);

  ASSERT_EQ(0, fclose(fp));
}

TEST(fseeko, espipe) {
  // Create a stream to read side of a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  FILE *fp = fdopen(fds[0], "r");
  ASSERT_NE(NULL, fp);
  ASSERT_EQ(0, close(fds[1]));

  // Seeking on a pipe should fail.
  ASSERT_EQ(-1, fseeko(fp, 10, SEEK_CUR));
  ASSERT_FALSE(ferror(fp));
  ASSERT_EQ(ESPIPE, errno);

  ASSERT_EQ(0, fclose(fp));
}

TEST(fseeko, example) {
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);

  // Move offset to an absolute position and place a character to grow
  // the file.
  ASSERT_EQ(0, fseeko(fp, 10, SEEK_SET));
  ASSERT_EQ(10, ftello(fp));
  ASSERT_EQ('a', putc('a', fp));

  // Seek beyond the end of the file.
  ASSERT_EQ(0, fseeko(fp, 10, SEEK_CUR));
  ASSERT_EQ(21, ftello(fp));

  // Seek relative to the end of the file.
  ASSERT_EQ(0, fseeko(fp, -5, SEEK_END));
  ASSERT_EQ(6, ftello(fp));

  ASSERT_EQ(0, fclose(fp));
}
