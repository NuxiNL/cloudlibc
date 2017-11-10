// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>
#include <testing.h>
#include <wchar.h>

TEST(wcsdup, hello) {
  wchar_t *copy = wcsdup(L"Hello");
  ASSERT_STREQ(L"Hello", copy);
  free(copy);
}
