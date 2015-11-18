// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <locale.h>
#include <testing.h>
#include <uchar.h>

TEST(mbrtoc16, ascii) {
  mbstate_t mbs = {};
  char16_t c16;

  // Valid character.
  ASSERT_EQ(1, mbrtoc16(&c16, "Foo", 3, &mbs));
  ASSERT_EQ(u'F', c16);

  // Null character.
  ASSERT_EQ(0, mbrtoc16(&c16, "", 1, &mbs));
  ASSERT_EQ(u'\0', c16);

  // Buffer too short.
  ASSERT_EQ((size_t)-2, mbrtoc16(&c16, "Some text", 0, &mbs));

  // Invalid character.
  ASSERT_EQ((size_t)-1, mbrtoc16(&c16, "€", 4, &mbs));
  ASSERT_EQ(EILSEQ, errno);
}

TEST(mbrtoc16, unicode) {
  mbstate_t mbs = {};
  char16_t c16;

  // Valid character.
  ASSERT_EQ(1, mbrtoc16_l(&c16, "Foo", 3, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(u'F', c16);

  // Piecewise consume an Unicode character.
  ASSERT_EQ((size_t)-2,
            mbrtoc16_l(&c16, "\xf0\x90", 2, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ((size_t)-2, mbrtoc16_l(&c16, "\x90", 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(1, mbrtoc16_l(&c16, "\xb7", 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(0xd801, c16);
  ASSERT_EQ((size_t)-3, mbrtoc16_l(&c16, "AAA", 3, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(0xdc37, c16);

  // Null character.
  ASSERT_EQ(0, mbrtoc16_l(&c16, "", 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(u'\0', c16);

  // Buffer too short.
  ASSERT_EQ((size_t)-2,
            mbrtoc16_l(&c16, "Some text", 0, &mbs, LC_C_UNICODE_LOCALE));
}
