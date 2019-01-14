// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <wctype.h>

#include "gtest/gtest.h"

TEST(iswgraph, examples) {
  ASSERT_FALSE(iswgraph(WEOF));

  ASSERT_TRUE(iswgraph(L'0'));
  ASSERT_TRUE(iswgraph(L'A'));
  ASSERT_TRUE(iswgraph(L'a'));
  ASSERT_TRUE(iswgraph(L'Å'));
  ASSERT_TRUE(iswgraph(L'Ω'));
  ASSERT_TRUE(iswgraph(L'д'));
  ASSERT_TRUE(iswgraph(L'.'));
  ASSERT_TRUE(iswgraph(L'€'));
  ASSERT_FALSE(iswgraph(L' '));
}

TEST(iswgraph, ascii) {
  locale_t locale = newlocale(LC_CTYPE_MASK, ".US-ASCII", 0);
  ASSERT_TRUE(iswgraph_l(L'0', locale));
  ASSERT_TRUE(iswgraph_l(L'A', locale));
  ASSERT_TRUE(iswgraph_l(L'a', locale));
  ASSERT_FALSE(iswgraph_l(L'Å', locale));
  ASSERT_FALSE(iswgraph_l(L'Ω', locale));
  ASSERT_FALSE(iswgraph_l(L'д', locale));
  ASSERT_TRUE(iswgraph_l(L'.', locale));
  ASSERT_FALSE(iswgraph_l(L'€', locale));
  ASSERT_FALSE(iswgraph_l(L' ', locale));
  freelocale(locale);
}
