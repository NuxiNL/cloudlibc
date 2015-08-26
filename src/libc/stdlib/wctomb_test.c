// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdlib.h>
#include <testing.h>

TEST(wctomb, examples) {
  // ASCII does not have a state dependency.
  ASSERT_EQ(0, wctomb(NULL, 123));

  // Valid input sequences.
  char c;
  ASSERT_EQ(1, wctomb(&c, L'\0'));
  ASSERT_EQ('\0', c);
  ASSERT_EQ(1, wctomb(&c, L'A'));
  ASSERT_EQ('A', c);

  // Non-ASCII input sequence.
  ASSERT_EQ(-1, wctomb((char *)42, L'€'));
  ASSERT_EQ(EILSEQ, errno);
}
