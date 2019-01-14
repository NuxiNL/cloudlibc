// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <locale.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(mbtowc, bad) {
  // Input sequence too short.
  ASSERT_EQ(-1, mbtowc(NULL, "", 0));
  ASSERT_EQ(EILSEQ, errno);
  ASSERT_EQ(-1, mbtowc_l(NULL, "", 0, LC_C_LOCALE));
  ASSERT_EQ(EILSEQ, errno);
  ASSERT_EQ(-1, mbtowc(NULL, "Hello", 0));
  ASSERT_EQ(EILSEQ, errno);
  ASSERT_EQ(-1, mbtowc_l(NULL, "Hello", 0, LC_C_LOCALE));
  ASSERT_EQ(EILSEQ, errno);
}

TEST(mbtowc, ascii) {
  // ASCII is not a state-dependent encoding.
  ASSERT_EQ(0, mbtowc(NULL, NULL, 12345));
  ASSERT_EQ(0, mbtowc_l(NULL, NULL, 12345, LC_C_LOCALE));

  // All lower codepoints should be mapped.
  wchar_t wc;
  char c = 0;
  ASSERT_EQ(0, mbtowc(&wc, &c, 12345));
  ASSERT_EQ(0, wc);
  ASSERT_EQ(0, mbtowc_l(&wc, &c, 12345, LC_C_LOCALE));
  ASSERT_EQ(0, wc);
  for (int i = 1; i < 128; ++i) {
    SCOPED_TRACE(i);
    c = i;
    ASSERT_EQ(1, mbtowc(&wc, &c, 12345));
    ASSERT_EQ(i, wc);
    ASSERT_EQ(1, mbtowc_l(&wc, &c, 12345, LC_C_LOCALE));
    ASSERT_EQ(i, wc);
  }
  // Everything above should throw EILSEQ.
  for (int i = 128; i < 256; ++i) {
    SCOPED_TRACE(i);
    c = i;
    ASSERT_EQ(-1, mbtowc(NULL, &c, 12345));
    ASSERT_EQ(EILSEQ, errno);
    ASSERT_EQ(-1, mbtowc_l(NULL, &c, 12345, LC_C_LOCALE));
    ASSERT_EQ(EILSEQ, errno);
  }
}

TEST(mbtowc, iso_8859_1) {
  locale_t locale = newlocale(LC_CTYPE_MASK, ".ISO-8859-1", 0);
  ASSERT_NE(NULL, locale);

  // ISO-8859-1 is not a state-dependent encoding.
  ASSERT_EQ(0, mblen_l(NULL, 12345, locale));

  // All lower codepoints should be mapped.
  char c = 0;
  ASSERT_EQ(0, mblen(&c, 12345));
  for (int i = 1; i < 256; ++i) {
    SCOPED_TRACE(i);
    c = i;
    ASSERT_EQ(1, mblen_l(&c, 12345, locale));
  }

  freelocale(locale);
}

TEST(mbtowc, euro) {
  // We cannot parse the character partially.
  char euro[] = "€";
  for (size_t i = 0; i < sizeof(euro) - 1; ++i) {
    ASSERT_EQ(-1, mbtowc_l(NULL, euro, i, LC_C_UNICODE_LOCALE));
    ASSERT_EQ(EILSEQ, errno);
  }

  // Character should be fully parsed.
  wchar_t wc;
  ASSERT_EQ(sizeof(euro) - 1,
            mbtowc_l(&wc, euro, sizeof(euro) - 1, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(L'€', wc);
  ASSERT_EQ(sizeof(euro) - 1,
            mbtowc_l(&wc, euro, sizeof(euro), LC_C_UNICODE_LOCALE));
  ASSERT_EQ(L'€', wc);
}
