// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wchar.h>

TEST(wcsnlen, null) {
  ASSERT_EQ(0, wcsnlen(NULL, 0));
  ASSERT_EQ(0, wcsnlen(L"", 100));
  ASSERT_EQ(7, wcsnlen(L"Σὲ γνωρίζω ἀπὸ τὴν κόψη", 7));
}
