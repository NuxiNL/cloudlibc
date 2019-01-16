// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

#include "gtest/gtest.h"

TEST(wcsnlen, null) {
  ASSERT_EQ(0, wcsnlen(NULL, 0));
  ASSERT_EQ(0, wcsnlen(L"", 100));
  ASSERT_EQ(7, wcsnlen(L"Σὲ γνωρίζω ἀπὸ τὴν κόψη", 7));
}
