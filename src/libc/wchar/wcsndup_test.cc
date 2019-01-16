// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>
#include <wchar.h>

#include "gtest/gtest.h"

TEST(wcsndup, null) {
  wchar_t *copy = wcsndup(NULL, 0);
  ASSERT_STREQ(L"", copy);
  free(copy);
}

TEST(wcsndup, hello) {
  wchar_t *copy = wcsndup(L"Hello, world", 5);
  ASSERT_STREQ(L"Hello", copy);
  free(copy);
}
