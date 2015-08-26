// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdlib.h>
#include <testing.h>

TEST(mblen, examples) {
  // ASCII does not have a state dependency.
  ASSERT_EQ(0, mblen(NULL, 123));

  // Buffer too short.
  ASSERT_EQ(-1, mblen((char *)42, 0));
  ASSERT_EQ(EILSEQ, errno);

  // Valid input sequences.
  ASSERT_EQ(0, mblen("\0", 1));
  ASSERT_EQ(1, mblen("Hello, world", 12));

  // Non-ASCII input sequence.
  ASSERT_EQ(-1, mblen("\xc1", 1));
  ASSERT_EQ(EILSEQ, errno);
}
