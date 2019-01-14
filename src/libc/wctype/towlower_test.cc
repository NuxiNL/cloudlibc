// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wctype.h>

#include "gtest/gtest.h"

TEST(towlower, examples) {
  ASSERT_EQ(WEOF, towlower(WEOF));

  ASSERT_EQ(L' ', towlower(L' '));
  ASSERT_EQ(L'€', towlower(L'€'));

  ASSERT_EQ(L'a', towlower(L'A'));
  ASSERT_EQ(L'a', towlower(L'a'));

  ASSERT_EQ(L'ä', towlower(L'Ä'));
  ASSERT_EQ(L'ä', towlower(L'ä'));

  ASSERT_EQ(L'λ', towlower(L'Λ'));
  ASSERT_EQ(L'λ', towlower(L'λ'));

  ASSERT_EQ(L'𐐷', towlower(L'𐐏'));
  ASSERT_EQ(L'𐐷', towlower(L'𐐷'));
}
