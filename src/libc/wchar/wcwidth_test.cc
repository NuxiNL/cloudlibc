// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>
#include <wctype.h>

#include "gtest/gtest.h"

TEST(wcwidth, iswprint) {
  ASSERT_EQ(0, wcwidth(L'\0'));
  for (wchar_t wc = 1; wc < 0x10ffff; ++wc) {
    SCOPED_TRACE(wc);
    ASSERT_EQ(iswprint(wc), wcwidth(wc) >= 0);
  }
}
