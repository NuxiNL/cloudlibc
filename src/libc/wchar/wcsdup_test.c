// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <testing.h>
#include <wchar.h>

TEST(wcsdup, hello) {
  wchar_t *copy = wcsdup(L"Hello");
  ASSERT_STREQ(L"Hello", copy);
  free(copy);
}
