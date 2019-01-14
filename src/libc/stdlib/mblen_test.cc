// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <locale.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(mblen, bad) {
  // Input sequence too short.
  ASSERT_EQ(-1, mblen("", 0));
  ASSERT_EQ(EILSEQ, errno);
  ASSERT_EQ(-1, mblen_l("", 0, LC_C_LOCALE));
  ASSERT_EQ(EILSEQ, errno);
  ASSERT_EQ(-1, mblen("Hello", 0));
  ASSERT_EQ(EILSEQ, errno);
  ASSERT_EQ(-1, mblen_l("Hello", 0, LC_C_LOCALE));
  ASSERT_EQ(EILSEQ, errno);
}

TEST(mblen, ascii) {
  // ASCII is not a state-dependent encoding.
  ASSERT_EQ(0, mblen(NULL, 12345));
  ASSERT_EQ(0, mblen_l(NULL, 12345, LC_C_LOCALE));

  // All lower codepoints should be mapped.
  char c = 0;
  ASSERT_EQ(0, mblen(&c, 12345));
  for (int i = 1; i < 128; ++i) {
    SCOPED_TRACE(i);
    c = i;
    ASSERT_EQ(1, mblen(&c, 12345));
    ASSERT_EQ(1, mblen_l(&c, 12345, LC_C_LOCALE));
  }
  // Everything above should throw EILSEQ.
  for (int i = 128; i < 256; ++i) {
    SCOPED_TRACE(i);
    c = i;
    ASSERT_EQ(-1, mblen(&c, 12345));
    ASSERT_EQ(EILSEQ, errno);
    ASSERT_EQ(-1, mblen_l(&c, 12345, LC_C_LOCALE));
    ASSERT_EQ(EILSEQ, errno);
  }
}

TEST(mblen, iso_8859_1) {
  locale_t locale = newlocale(LC_CTYPE_MASK, ".ISO-8859-1", 0);
  ASSERT_NE(NULL, locale);

  // ISO-8859-1 is not a state-dependent encoding.
  ASSERT_EQ(0, mbtowc_l(NULL, NULL, 12345, locale));

  // All lower codepoints should be mapped.
  wchar_t wc;
  char c = 0;
  ASSERT_EQ(0, mbtowc_l(&wc, &c, 12345, locale));
  ASSERT_EQ(0, wc);
  for (int i = 1; i < 256; ++i) {
    SCOPED_TRACE(i);
    c = i;
    ASSERT_EQ(1, mbtowc_l(&wc, &c, 12345, locale));
    ASSERT_EQ(i, wc);
  }

  freelocale(locale);
}

TEST(mblen, euro) {
  // We cannot parse the character partially.
  char euro[] = "€";
  for (size_t i = 0; i < sizeof(euro) - 1; ++i) {
    ASSERT_EQ(-1, mblen_l(euro, i, LC_C_UNICODE_LOCALE));
    ASSERT_EQ(EILSEQ, errno);
  }

  // Character should be fully parsed.
  ASSERT_EQ(sizeof(euro) - 1,
            mblen_l(euro, sizeof(euro) - 1, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(sizeof(euro) - 1, mblen_l(euro, sizeof(euro), LC_C_UNICODE_LOCALE));
}
