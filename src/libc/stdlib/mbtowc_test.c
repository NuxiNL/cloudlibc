// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdlib.h>
#include <testing.h>

TEST(mbtowc, examples) {
  // ASCII does not have a state dependency.
  ASSERT_EQ(0, mbtowc(NULL, NULL, 123));

  // Buffer too short.
  ASSERT_EQ(-1, mbtowc(NULL, (char *)42, 0));
  ASSERT_EQ(EILSEQ, errno);

  // Valid input sequences.
  wchar_t wc;
  ASSERT_EQ(0, mbtowc(NULL, "\0", 1));
  ASSERT_EQ(0, mbtowc(&wc, "\0", 1));
  ASSERT_EQ(L'\0', wc);
  ASSERT_EQ(1, mbtowc(NULL, "Hello, world", 12));
  ASSERT_EQ(1, mbtowc(&wc, "Hello, world", 12));
  ASSERT_EQ(L'H', wc);

  // Non-ASCII input sequence.
  ASSERT_EQ(-1, mbtowc(NULL, "\xc1", 1));
  ASSERT_EQ(EILSEQ, errno);
}
