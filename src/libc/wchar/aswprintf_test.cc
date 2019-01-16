// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <wchar.h>

#include "gtest/gtest.h"

TEST(aswprintf, good) {
  // No character set conversion performed.
  wchar_t *str;
  ASSERT_EQ(11, aswprintf(&str, L"%S %s", L"H€llø", "world"));
  ASSERT_STREQ(L"H€llø world", str);
  free(str);

  // UTF-8 to Unicode conversion performed.
  locale_t locale = newlocale(LC_CTYPE_MASK, ".UTF-8", 0);
  ASSERT_NE((locale_t)0, locale);
  ASSERT_EQ(11, aswprintf_l(&str, locale, L"%s %s", "H€llø", "world"));
  ASSERT_STREQ(L"H€llø world", str);
  free(str);
  freelocale(locale);
}

TEST(aswprintf, bad) {
  // The non-ASCII symbols in H€llø cannot be converted.
  ASSERT_EQ(-1, aswprintf(NULL, L"%s %S", "H€llø", L"world"));
  ASSERT_EQ(EILSEQ, errno);
}
