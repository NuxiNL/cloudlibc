// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wctype.h>

TEST(iswxdigit, examples) {
  ASSERT_TRUE(iswxdigit(L'0'));
  ASSERT_TRUE(iswxdigit(L'1'));
  ASSERT_TRUE(iswxdigit(L'2'));
  ASSERT_TRUE(iswxdigit(L'3'));
  ASSERT_TRUE(iswxdigit(L'4'));
  ASSERT_TRUE(iswxdigit(L'5'));
  ASSERT_TRUE(iswxdigit(L'6'));
  ASSERT_TRUE(iswxdigit(L'7'));
  ASSERT_TRUE(iswxdigit(L'8'));
  ASSERT_TRUE(iswxdigit(L'9'));

  ASSERT_TRUE(iswxdigit(L'A'));
  ASSERT_TRUE(iswxdigit(L'a'));
  ASSERT_TRUE(iswxdigit(L'B'));
  ASSERT_TRUE(iswxdigit(L'b'));
  ASSERT_TRUE(iswxdigit(L'C'));
  ASSERT_TRUE(iswxdigit(L'c'));
  ASSERT_TRUE(iswxdigit(L'D'));
  ASSERT_TRUE(iswxdigit(L'd'));
  ASSERT_TRUE(iswxdigit(L'E'));
  ASSERT_TRUE(iswxdigit(L'e'));
  ASSERT_TRUE(iswxdigit(L'F'));
  ASSERT_TRUE(iswxdigit(L'f'));

  ASSERT_FALSE(iswxdigit(L'g'));
  ASSERT_FALSE(iswxdigit(L'１'));
}
