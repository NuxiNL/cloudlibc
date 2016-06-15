// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <stdlib.h>
#include <testing.h>

TEST(wctomb, ascii) {
  // ASCII is not a state-dependent encoding.
  ASSERT_EQ(0, wctomb(NULL, L'€'));
  ASSERT_EQ(0, wctomb_l(NULL, L'€', LC_C_LOCALE));

  // All lower codepoints should be mapped.
  for (int i = 0; i < 128; ++i) {
    SCOPED_NOTE(i, {
      char c;
      ASSERT_EQ(1, wctomb(&c, i));
      ASSERT_EQ(i, c);
      ASSERT_EQ(1, wctomb_l(&c, i, LC_C_LOCALE));
      ASSERT_EQ(i, c);
    });
  }
  // Everything above should throw EILSEQ.
  for (int i = 128; i < 256; ++i) {
    SCOPED_NOTE(i, {
      char c;
      ASSERT_EQ(-1, wctomb(&c, i));
      ASSERT_EQ(EILSEQ, errno);
      ASSERT_EQ(-1, wctomb_l(&c, i, LC_C_LOCALE));
      ASSERT_EQ(EILSEQ, errno);
    });
  }
}

TEST(wctomb, iso_8859_1) {
  locale_t locale = newlocale(LC_CTYPE_MASK, ".ISO-8859-1", 0);
  ASSERT_NE(NULL, locale);

  // ISO-8859-1 is not a state-dependent encoding.
  ASSERT_EQ(0, wctomb_l(NULL, L'€', locale));

  // All lower codepoints should be mapped.
  for (int i = 0; i < 256; ++i) {
    SCOPED_NOTE(i, {
      char c;
      ASSERT_EQ(1, wctomb_l(&c, i, locale));
      ASSERT_EQ(i, c);
    });
  }

  freelocale(locale);
}

TEST(wctomb, euro) {
  char buf[MB_LEN_MAX];
  ASSERT_EQ(sizeof("€") - 1, wctomb_l(buf, L'€', LC_C_UNICODE_LOCALE));
  ASSERT_ARREQ("€", buf, sizeof("€") - 1);
}
