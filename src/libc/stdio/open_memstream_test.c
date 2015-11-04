// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdio.h>
#include <stdlib.h>
#include <testing.h>

TEST(open_memstream, posix_example) {
  // Example taken the description of open_memstream().
  char *buf;
  size_t len;
  FILE *stream = open_memstream(&buf, &len);
  ASSERT_NE(NULL, stream);
  ASSERT_EQ(14, fprintf(stream, "hello my world"));
  ASSERT_EQ(0, fflush(stream));
  ASSERT_STREQ("hello my world", buf);
  ASSERT_EQ(14, len);
  ASSERT_EQ(14, ftello(stream));
  ASSERT_EQ(0, fseeko(stream, 0, SEEK_SET));
  ASSERT_EQ(8, fprintf(stream, "good-bye"));
  ASSERT_EQ(0, fseeko(stream, 14, SEEK_SET));
  ASSERT_EQ(0, fclose(stream));
  ASSERT_STREQ("good-bye world", buf);
  ASSERT_EQ(14, len);
  free(buf);
}

// TODO(ed): Add more tests.
