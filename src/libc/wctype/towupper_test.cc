// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <stddef.h>
#include <wctype.h>
#include <iterator>

#include "gtest/gtest.h"

TEST(towupper, examples) {
  ASSERT_EQ(WEOF, towupper(WEOF));

  ASSERT_EQ(L' ', towupper(L' '));
  ASSERT_EQ(L'€', towupper(L'€'));

  ASSERT_EQ(L'A', towupper(L'A'));
  ASSERT_EQ(L'A', towupper(L'a'));

  ASSERT_EQ(L'Ä', towupper(L'Ä'));
  ASSERT_EQ(L'Ä', towupper(L'ä'));

  ASSERT_EQ(L'Ÿ', towupper(L'Ÿ'));
  ASSERT_EQ(L'Ÿ', towupper(L'ÿ'));

  ASSERT_EQ(L'Λ', towupper(L'Λ'));
  ASSERT_EQ(L'Λ', towupper(L'λ'));

  ASSERT_EQ(L'𐐏', towupper(L'𐐏'));
  ASSERT_EQ(L'𐐏', towupper(L'𐐷'));
}

TEST(towupper, iso_8859_1) {
  wchar_t in[] = L"l'Haÿ-les-Roses";
  wchar_t out[std::size(in)];

  // Convert to upper case.
  locale_t locale = newlocale(LC_CTYPE_MASK, ".ISO-8859-1", 0);
  for (size_t i = 0; i < std::size(in); ++i)
    out[i] = towupper_l(in[i], locale);
  freelocale(locale);

  // Ÿ is not part of ISO-8859-1, so it should remain lower case.
  ASSERT_STREQ(L"L'HAÿ-LES-ROSES", out);
}
