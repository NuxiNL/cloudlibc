// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdio.h>
#include <stdlib.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(open_memstream, cursor) {
  char *buf;
  size_t len;
  FILE *stream = open_memstream(&buf, &len);
  ASSERT_NE(NULL, stream);
  ASSERT_LE(0, fputs("Hello", stream));

  // The value of sizep depends on the cursor position.
  for (size_t i = 0; i < 5; ++i) {
    ASSERT_EQ(0, fseeko(stream, i, SEEK_SET));
    ASSERT_EQ(0, fflush(stream));
    ASSERT_STREQ("Hello", buf);
    ASSERT_EQ(i, len);
  }

  // It should never exceed the size of the buffer.
  for (size_t i = 5; i < 10; ++i) {
    ASSERT_EQ(0, fseeko(stream, i, SEEK_SET));
    ASSERT_EQ(0, fflush(stream));
    ASSERT_STREQ("Hello", buf);
    ASSERT_EQ(5, len);
  }

  ASSERT_EQ(0, fclose(stream));
  free(buf);
}

TEST(open_memstream, empty) {
  // Empty string.
  char *buf;
  size_t len;
  FILE *stream = open_memstream(&buf, &len);
  ASSERT_NE(NULL, stream);
  ASSERT_EQ(0, fclose(stream));
  ASSERT_STREQ("", buf);
  ASSERT_EQ(0, len);
  free(buf);
}

TEST(open_memstream, posix_example) {
  // Example taken from the description of open_memstream().
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

TEST(open_memstream, sparse) {
  // Sparse area should be filled with zero bytes.
  char *buf;
  size_t len;
  FILE *stream = open_memstream(&buf, &len);
  ASSERT_NE(NULL, stream);
  ASSERT_EQ('a', putc('a', stream));
  ASSERT_EQ(0, fseeko(stream, 2, SEEK_CUR));
  ASSERT_EQ('b', putc('b', stream));
  ASSERT_EQ(0, fseeko(stream, 2, SEEK_CUR));
  ASSERT_EQ('c', putc('c', stream));
  ASSERT_EQ(7, ftello(stream));
  ASSERT_EQ(0, fclose(stream));
  ASSERT_EQ(0, memcmp("a\x00\x00"
                      "b\x00\x00"
                      "c\x00",
                      buf, 8));
  ASSERT_EQ(7, len);
  free(buf);
}
