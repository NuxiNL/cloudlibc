// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdio.h>
#include <testing.h>
#include <unistd.h>

TEST(fgetpos, eoverflow) {
  // Create a stream with a negative file position indicator.
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);
  ASSERT_EQ('a', ungetc('a', fp));

  // It should be impossible to get the position in the stream. The old
  // position should remain unaffected.
  ASSERT_EQ(-1, fgetpos(fp, NULL));
  ASSERT_EQ(EOVERFLOW, errno);
}

TEST(fgetpos, espipe) {
  // Create a stream to the read side of a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  FILE *fp = fdopen(fds[0], "r");
  ASSERT_NE(NULL, fp);

  // It should be impossible to obtain the position.
  ASSERT_EQ(-1, fgetpos(fp, NULL));
  ASSERT_EQ(ESPIPE, errno);

  ASSERT_EQ(0, fclose(fp));
  ASSERT_EQ(0, close(fds[1]));
}

TEST(fgetpos, success) {
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);
  fpos_t pos;
  ASSERT_EQ(0, fgetpos(fp, &pos));
  ASSERT_EQ(0, fclose(fp));
}
