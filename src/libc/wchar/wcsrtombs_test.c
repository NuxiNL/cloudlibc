// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <locale.h>
#include <testing.h>
#include <wchar.h>

TEST(wcsrtombs, ascii_ok) {
  // String should be fully converted.
  const wchar_t *src = L"Hello, world";
  char dst[13];
  static const mbstate_t initial_mbstate;
  mbstate_t mbs = initial_mbstate;
  ASSERT_EQ(sizeof(dst) - 1, wcsrtombs(dst, &src, sizeof(dst), &mbs));
  ASSERT_EQ(NULL, src);
  ASSERT_STREQ("Hello, world", dst);
}

TEST(wcsrtombs, ascii_bad_rightbefore) {
  // Conversion should stop right before the illegal character.
  const wchar_t *src = L"Hello, wørld";
  char dst[8];
  static const mbstate_t initial_mbstate;
  mbstate_t mbs = initial_mbstate;
  ASSERT_EQ(sizeof(dst), wcsrtombs(dst, &src, sizeof(dst), &mbs));
  ASSERT_STREQ(L"ørld", src);
  ASSERT_ARREQ("Hello, w", dst, 8);
}

TEST(wcsrtombs, ascii_bad) {
  // Conversion should stop due to the illegal character.
  const wchar_t *src = L"Hello, wørld";
  char dst[9];
  static const mbstate_t initial_mbstate;
  mbstate_t mbs = initial_mbstate;
  ASSERT_EQ((size_t)-1, wcsrtombs(dst, &src, sizeof(dst), &mbs));
  ASSERT_EQ(EILSEQ, errno);
  ASSERT_STREQ(L"ørld", src);
}

TEST(wcsrtombs, ascii_null_ok) {
  // Length should be computed. Source should be left unmodified.
  const wchar_t *src = L"Hello, world";
  static const mbstate_t initial_mbstate;
  mbstate_t mbs = initial_mbstate;
  ASSERT_EQ(12, wcsrtombs(NULL, &src, 0, &mbs));
  ASSERT_STREQ(L"Hello, world", src);
}

TEST(wcsrtombs, ascii_null_bad) {
  // Length cannot be computed. Source should be left unmodified.
  const wchar_t *src = L"Hello, wørld";
  static const mbstate_t initial_mbstate;
  mbstate_t mbs = initial_mbstate;
  ASSERT_EQ((size_t)-1, wcsrtombs(NULL, &src, 0, &mbs));
  ASSERT_STREQ(L"Hello, wørld", src);
}

TEST(wcsrtombs, unicode_ok) {
  // String should be fully converted.
  const wchar_t *src = L"ℕ ⊆ ℕ₀ ⊂ ℤ ⊂ ℚ ⊂ ℝ ⊂ ℂ";
  char dst[47];
  static const mbstate_t initial_mbstate;
  mbstate_t mbs = initial_mbstate;
  ASSERT_EQ(sizeof(dst) - 1,
            wcsrtombs_l(dst, &src, sizeof(dst), &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(NULL, src);
  ASSERT_STREQ("ℕ ⊆ ℕ₀ ⊂ ℤ ⊂ ℚ ⊂ ℝ ⊂ ℂ", dst);
}

TEST(wcsrtombs, iso_8859_1) {
  const wchar_t *src = L"§ 62 Grundsatz der Verhältnismäßigkeit";
  char dst[39];
  static const mbstate_t initial_mbstate;
  mbstate_t mbs = initial_mbstate;
  locale_t locale = newlocale(LC_CTYPE_MASK, "de_DE.ISO8859-1", 0);
  ASSERT_NE(0, locale);
  ASSERT_EQ(sizeof(dst) - 1, wcsrtombs_l(dst, &src, sizeof(dst), &mbs, locale));
  ASSERT_EQ(NULL, src);
  ASSERT_STREQ("\xa7 62 Grundsatz der Verh\xe4ltnism\xe4\xdfigkeit", dst);
  freelocale(locale);
}

TEST(wcsrtombs, iso_8859_15) {
  const wchar_t *src = L"«Nur für eine Nacht» von PIERRE LOUŸS: € 36,95";
  char dst[47];
  static const mbstate_t initial_mbstate;
  mbstate_t mbs = initial_mbstate;
  locale_t locale = newlocale(LC_CTYPE_MASK, "de_DE.ISO8859-15", 0);
  ASSERT_NE(0, locale);
  ASSERT_EQ(sizeof(dst) - 1, wcsrtombs_l(dst, &src, sizeof(dst), &mbs, locale));
  ASSERT_EQ(NULL, src);
  ASSERT_STREQ("\xabNur f\xfcr eine Nacht\xbb von PIERRE LOU\xbeS: \xa4 36,95",
               dst);
  freelocale(locale);
}
