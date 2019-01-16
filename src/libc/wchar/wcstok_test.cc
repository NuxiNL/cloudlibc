// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

#include "gtest/gtest.h"

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
