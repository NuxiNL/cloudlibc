// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <locale.h>
#include <testing.h>
#include <uchar.h>
#include <wchar.h>

TEST(mbrtoc32, ascii) {
  static const mbstate_t initial_mbstate;
  mbstate_t mbs = initial_mbstate;
  char32_t c32;

  // Valid character.
  ASSERT_EQ(1, mbrtoc32(&c32, "Foo", 3, &mbs));
  ASSERT_EQ(U'F', c32);
  ASSERT_NE(0, mbsinit(&mbs));

  // Null character.
  ASSERT_EQ(0, mbrtoc32(&c32, "", 1, &mbs));
  ASSERT_EQ(U'\0', c32);
  ASSERT_NE(0, mbsinit(&mbs));

  // Buffer too short.
  ASSERT_EQ((size_t)-2, mbrtoc32(&c32, "Some text", 0, &mbs));
  ASSERT_NE(0, mbsinit(&mbs));

  // Invalid character.
  ASSERT_EQ((size_t)-1, mbrtoc32(&c32, "€", 4, &mbs));
  ASSERT_EQ(EILSEQ, errno);
  ASSERT_NE(0, mbsinit(&mbs));
}

TEST(mbrtoc32, unicode) {
  static const mbstate_t initial_mbstate;
  mbstate_t mbs = initial_mbstate;
  char32_t c32;

  // Valid character.
  ASSERT_EQ(1, mbrtoc32_l(&c32, "Foo", 3, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(U'F', c32);
  ASSERT_NE(0, mbsinit(&mbs));

  // Piecewise consume an Unicode character.
  ASSERT_EQ((size_t)-2,
            mbrtoc32_l(&c32, "\xf0\x90", 2, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(0, mbsinit(&mbs));
  ASSERT_EQ((size_t)-2, mbrtoc32_l(&c32, "\x90", 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(0, mbsinit(&mbs));
  ASSERT_EQ(1, mbrtoc32_l(&c32, "\xb7", 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(U'𐐷', c32);
  ASSERT_NE(0, mbsinit(&mbs));

  // Null character.
  ASSERT_EQ(0, mbrtoc32_l(&c32, "", 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(U'\0', c32);
  ASSERT_NE(0, mbsinit(&mbs));

  // Buffer too short.
  ASSERT_EQ((size_t)-2,
            mbrtoc32_l(&c32, "Some text", 0, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_NE(0, mbsinit(&mbs));
}
