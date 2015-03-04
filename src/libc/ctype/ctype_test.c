// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <ctype.h>
#include <wctype.h>

#include <testing.h>

TEST(ctype, all) {
  // All ASCII values should behave identically to the wide-character
  // counterparts.
  for (int ch = 0; ch <= 127; ++ch) {
    SCOPED_NOTE(ch, {
      ASSERT_EQ(iswalnum(ch), isalnum(ch));
      ASSERT_EQ(iswalpha(ch), isalpha(ch));
      ASSERT_EQ(iswblank(ch), isblank(ch));
      ASSERT_EQ(iswcntrl(ch), iscntrl(ch));
      ASSERT_EQ(iswdigit(ch), isdigit(ch));
      ASSERT_EQ(iswgraph(ch), isgraph(ch));
      ASSERT_EQ(iswlower(ch), islower(ch));
      ASSERT_EQ(iswprint(ch), isprint(ch));
      ASSERT_EQ(iswpunct(ch), ispunct(ch));
      ASSERT_EQ(iswspace(ch), isspace(ch));
      ASSERT_EQ(iswupper(ch), isupper(ch));
      ASSERT_EQ(iswxdigit(ch), isxdigit(ch));
      ASSERT_EQ(towlower(ch), tolower(ch));
      ASSERT_EQ(towupper(ch), toupper(ch));
    });
  }

  // Everything outside of ASCII should be invalid.
  for (int ch = 128; ch <= 255; ++ch) {
    SCOPED_NOTE(ch, {
      ASSERT_EQ(0, isalnum(ch));
      ASSERT_EQ(0, isalpha(ch));
      ASSERT_EQ(0, isblank(ch));
      ASSERT_EQ(0, iscntrl(ch));
      ASSERT_EQ(0, isdigit(ch));
      ASSERT_EQ(0, isgraph(ch));
      ASSERT_EQ(0, islower(ch));
      ASSERT_EQ(0, isprint(ch));
      ASSERT_EQ(0, ispunct(ch));
      ASSERT_EQ(0, isspace(ch));
      ASSERT_EQ(0, isupper(ch));
      ASSERT_EQ(0, isxdigit(ch));
      ASSERT_EQ(ch, tolower(ch));
      ASSERT_EQ(ch, toupper(ch));
    });
  }
}
