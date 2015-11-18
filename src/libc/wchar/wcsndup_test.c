// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <testing.h>
#include <wchar.h>

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
