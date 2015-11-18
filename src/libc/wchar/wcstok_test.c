// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wchar.h>

TEST(wcstok, example) {
  wchar_t line[] = L"LINE  TO BE\t\tSEPARATED\n";
  const wchar_t *split = L" \t\n";
  wchar_t *lastws;
  ASSERT_STREQ(L"LINE", wcstok(line, split, &lastws));
  ASSERT_STREQ(L"TO", wcstok(NULL, split, &lastws));
  ASSERT_STREQ(L"BE", wcstok(NULL, split, &lastws));
  ASSERT_STREQ(L"SEPARATED", wcstok(NULL, split, &lastws));
  ASSERT_EQ(NULL, wcstok(NULL, split, &lastws));
}
