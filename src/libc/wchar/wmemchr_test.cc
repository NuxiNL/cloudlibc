// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>
#include <iterator>

#include "gtest/gtest.h"

TEST(wmemchr, null) {
  ASSERT_EQ(NULL, wmemchr((wchar_t *)NULL, L'A', 0));
}

TEST(wmemchr, match) {
  wchar_t buf[] = L"Foo bar baz";
  ASSERT_EQ(buf + 5, wmemchr(buf, L'a', std::size(buf)));
}

TEST(wmemchr, nomatch) {
  wchar_t buf[] = L"Foo bar baz";
  ASSERT_EQ(NULL, wmemchr(buf, L'x', std::size(buf)));
}
